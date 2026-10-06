#include "CustomDimensionManager.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"
#include "more_dimensions/core/dimension/CustomDimensionConfig.h"

#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"

#include "mc/util/NameIdStore.h"
#include "mc/world/level/DimensionManager.h"
#include "mc/world/level/GeneratorType.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/dimension/DimensionDefinitionGroup.h"
#include "mc/world/level/dimension/DimensionIdType.h"
#include "mc/world/level/dimension/VanillaDimensions.h"

#include <optional>

namespace more_dimensions {

auto& loggerMoreDimMag = MoreDimension::getInstance().getSelf().getLogger();

struct CustomDimensionManager::Impl {
    std::mutex mMapMutex;

    struct DimensionInfo {
        CompoundTag nbt;
    };
    std::unordered_map<std::string, DimensionInfo> customDimensionMap;
};

CustomDimensionManager::CustomDimensionManager() : impl(std::make_unique<Impl>()) {
    std::lock_guard lock{impl->mMapMutex};
    CustomDimensionConfig::setDimensionConfigPath();
    CustomDimensionConfig::loadConfigFile();
    for (auto& [name, info] : CustomDimensionConfig::getConfig().dimensionList) {
        auto nbtTag = CompoundTag::fromSnbt(info.sNbt);
        if (!nbtTag) {
            loggerMoreDimMag.error("Failed to parse NBT from config for dimension: {}, skipping", name);
            continue;
        }
        impl->customDimensionMap.emplace(name, Impl::DimensionInfo{*nbtTag});
    }
}

CustomDimensionManager::~CustomDimensionManager() = default;

CustomDimensionManager& CustomDimensionManager::getInstance() {
    static CustomDimensionManager instance{};
    return instance;
}

namespace {
std::optional<std::string> normalizeDimensionName(std::string const& dimName) {
    auto lowered = dimName;
    for (char& c : lowered) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }
    auto sep = lowered.find(':');
    if (sep == std::string::npos || lowered.find(':', sep + 1) != std::string::npos || sep == 0
        || sep + 1 == lowered.size()) {
        return std::nullopt;
    }
    auto namespaceName = std::string_view(lowered).substr(0, sep);
    auto name          = std::string_view(lowered).substr(sep + 1);
    if (namespaceName == "minecraft") {
        return std::nullopt;
    }
    if (namespaceName.find_first_of(" \t\n\r") != std::string_view::npos
        || name.find_first_of(" \t\n\r") != std::string_view::npos) {
        return std::nullopt;
    }
    return lowered;
}

// Value of Util::NameIdStore<T>::INVALID_ID in BDS (DimensionIdType is a ushort)
constexpr ushort kInvalidNameId = 0xFFFF;

/// @brief Reproduces the behaviour of Util::NameIdStore<DimensionIdType>::getOrCreateId from
/// bedrock_server.
///
/// LeviLamina's generated header exposes only the member variables of NameIdStore, not its
/// methods, so the logic is reimplemented here straight from the disassembly:
///   1) look in the already claimed mNameIdMap;
///   2) then in mUnclaimedNameIdMap, which holds mappings loaded from the persisted name-id table
///      that have not been claimed yet; a hit gets claimed;
///   3) only if neither matches, allocate from mNextCustomId; running past mMaxId means the id
///      range is exhausted.
DimensionIdType getOrCreateNameId(Util::NameIdStore<DimensionIdType>& store, std::string const& name) {
    if (auto it = store.mNameIdMap.mRight.find(name); it != store.mNameIdMap.mRight.end()) {
        return it->second;
    }

    if (auto it = store.mUnclaimedNameIdMap.mRight.find(name); it != store.mUnclaimedNameIdMap.mRight.end()) {
        auto id = it->second;
        // Claim it: register it in mNameIdMap and drop it from mUnclaimedNameIdMap.
        // (Not using BidirectionalUnorderedMap::eraser, which erases the right map before reading
        // the value back out of it, so the original value is already gone by then.)
        store.mUnclaimedNameIdMap.mLeft.erase(id);
        store.mUnclaimedNameIdMap.mRight.erase(name);
        store.mNameIdMap.insert_or_assign(name, id);
        return id;
    }

    if (store.mNextCustomId.mValue > store.mMaxId.mValue) {
        return DimensionIdType{kInvalidNameId};
    }

    DimensionIdType id{static_cast<ushort>(store.mNextCustomId.mValue++)};
    store.mNameIdMap.insert_or_assign(name, id);
    return id;
}

/// @brief Returns the dimension id for name, allocating and registering one if BDS does not know
/// the name yet. Equivalent to 26.20's
/// DimensionManager::serverRegisterCustomDimension(...).value_or(getDimensionId(...)).
DimensionType resolveDimensionId(DimensionManager& dimensionManager, std::string const& name, CompoundTag const& data) {
    auto id = getOrCreateNameId(*dimensionManager.mDimensionNameIdStore, name);
    if (id.mValue == kInvalidNameId) {
        return VanillaDimensions::Undefined();
    }

    DimensionType dimId{id.mValue};

    // An id alone is not enough: the dimension definition also has to go into
    // mDimensionDefinitionGroup. That group is sent to clients in DimensionDataPacket, and a
    // client that does not know the dimension cannot receive its chunks (which shows up as being
    // stuck on the loading screen). On top of that, mMinY/mHeightRange here must match the
    // Dimension's own height range, otherwise the client interprets the chunks using the wrong
    // range (globally shifted terrain, or an entirely void dimension). Note that mHeightRange is
    // a span, not a maximum: see OverworldDimension, where max = mMinY + mHeightRange.
    // The default is the overworld range {-64, 320}, which is what every dimension in src/test
    // uses. A dimension that needs a different range has to persist dimensionMinY /
    // dimensionHeightRange from its own generateNewData(); SimpleCustomDimension does exactly
    // that from heightRangeFor().
    // mPackId / mDefaultBiome are left at their defaults.
    DimensionDefinitionGroup::DimensionDefinition definition{};
    definition.mMinY = data.contains("dimensionMinY") ? static_cast<int>(data["dimensionMinY"]) : -64;
    definition.mHeightRange =
        data.contains("dimensionHeightRange") ? static_cast<int>(data["dimensionHeightRange"]) : 384;
    definition.mGeneratorType = GeneratorType::Void;
    definition.mDimensionType = dimId;

    // Same as 26.20: only register the dimension factory if the definition group accepted it
    if (dimensionManager._registerDimensionWithDimensionDefinitionGroup(name, definition)) {
        dimensionManager._registerCustomDimensionWithFactory(name, dimId);
    }
    return dimId;
}
} // namespace

DimensionType CustomDimensionManager::getDimensionIdFromName(std::string const& dimName) {
    return VanillaDimensions::fromString(dimName);
}

DimensionType
CustomDimensionManager::addSimpleDimension(std::string const& dimName, uint seed, GeneratorType generatorType) {
    return addDimension<SimpleCustomDimension>(dimName, seed, generatorType);
}

DimensionType CustomDimensionManager::addDimension(
    std::string const&                  dimName,
    std::function<DimensionFactoryT>    factory,
    std::function<CompoundTag()> const& data
) {
    std::lock_guard lock{impl->mMapMutex};
    auto            level = ll::service::getLevel();
    if (!level) {
        throw std::runtime_error("Level is nullptr, cannot registry new dimension " + dimName);
    }

    auto normalizedName = normalizeDimensionName(dimName);
    if (!normalizedName) {
        loggerMoreDimMag.error(
            "Invalid dimension name: {}, must be in namespace:name format and must not use the minecraft namespace",
            dimName
        );
        throw std::runtime_error("Invalid dimension name: " + dimName);
    }

    Impl::DimensionInfo info;
    bool                newDim = false;
    if (impl->customDimensionMap.contains(*normalizedName)) {
        info = impl->customDimensionMap.at(*normalizedName);
        loggerMoreDimMag
            .info("The dimension already has persisted data. name: {}, \ndata: {}", dimName, info.nbt.toSnbt());
    } else {
        info.nbt = data();
        newDim   = true;
        loggerMoreDimMag.info("registry new dimension data, name: {}, \ndata: {}", dimName, info.nbt.toSnbt());
    }

    auto& dimensionManager = level->getDimensionManager();
    auto  dimId            = resolveDimensionId(dimensionManager, *normalizedName, info.nbt);
    if (dimId == VanillaDimensions::Undefined()) {
        throw std::runtime_error("Failed to registry new dimension " + dimName);
    }

    level->getDimensionFactory().mFactoryMap.insert_or_assign(
        dimName,
        [dimName, info, dimId, factory = std::move(factory)](DerivedDimensionArguments&& arguments)
            -> OwnerPtr<Dimension> {
            loggerMoreDimMag.debug("Create dimension, name: {}, id: {}", dimName, static_cast<int>(dimId));
            return factory(DimensionFactoryInfo{arguments, info.nbt, dimId});
        }
    );
    loggerMoreDimMag.info("registry dimension, name: {}, id: {}", dimName, static_cast<int>(dimId));

    if (newDim) {
        impl->customDimensionMap.emplace(*normalizedName, info);
        CustomDimensionConfig::getConfig().dimensionList.emplace(
            *normalizedName,
            CustomDimensionConfig::Config::Info{
                .dimId = static_cast<int>(dimId),
                .sNbt  = info.nbt.toSnbt(SnbtFormat::Minimize)
            }
        );
        CustomDimensionConfig::saveConfigFile();
    }

    ll::command::CommandRegistrar::getInstance(false).addEnumValues(
        "Dimension",
        {
            {dimName, dimId}
    },
        Bedrock::type_id<CommandRegistry, DimensionType>()
    );

    return dimId;
}
} // namespace more_dimensions
