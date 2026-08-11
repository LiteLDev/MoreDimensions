#include "CustomDimensionManager.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"
#include "more_dimensions/core/dimension/CustomDimensionConfig.h"

#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"

#include "mc/world/level/DimensionManager.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/Dimension.h"
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
} // namespace

DimensionType CustomDimensionManager::getDimensionIdFromName(std::string const& dimName) {
    return VanillaDimensions::fromString(dimName);
}

DimensionType CustomDimensionManager::addSimpleDimension(
    std::string const& dimName,
    uint               seed,
    GeneratorType      generatorType
) {
    return addDimension<SimpleCustomDimension>(dimName, seed, generatorType);
}

DimensionType CustomDimensionManager::addDimension(
    std::string const&                  dimName,
    std::function<DimensionFactoryT>    factory,
    std::function<CompoundTag()> const& data
) {
    std::lock_guard lock{impl->mMapMutex};
    auto           level = ll::service::getLevel();
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
        loggerMoreDimMag.info(
            "The dimension already has persisted data. name: {}, \ndata: {}",
            dimName,
            info.nbt.toSnbt()
        );
    } else {
        info.nbt = data();
        newDim   = true;
        loggerMoreDimMag.info("registry new dimension data, name: {}, \ndata: {}", dimName, info.nbt.toSnbt());
    }

    auto& dimensionManager = level->getDimensionManager();
    auto  dimId = dimensionManager.serverRegisterCustomDimension(dimName).value_or(dimensionManager.getDimensionId(dimName));
    if (dimId == VanillaDimensions::Undefined()) {
        throw std::runtime_error("Failed to registry new dimension " + dimName);
    }

    level->getDimensionFactory().mFactoryMap.insert_or_assign(
        dimName,
        [dimName, info, dimId, factory = std::move(factory)](
            DerivedDimensionArguments&& arguments
        ) -> OwnerPtr<Dimension> {
            loggerMoreDimMag.debug("Create dimension, name: {}, id: {}", dimName, static_cast<int>(dimId));
            return factory(DimensionFactoryInfo{arguments, info.nbt, dimId});
        }
    );
    loggerMoreDimMag.info("registry dimension, name: {}, id: {}", dimName, static_cast<int>(dimId));

    if (newDim) {
        impl->customDimensionMap.emplace(*normalizedName, info);
        CustomDimensionConfig::getConfig().dimensionList.emplace(
            *normalizedName,
            CustomDimensionConfig::Config::Info{.dimId = dimId.value(), .sNbt = info.nbt.toSnbt(SnbtFormat::Minimize)}
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
