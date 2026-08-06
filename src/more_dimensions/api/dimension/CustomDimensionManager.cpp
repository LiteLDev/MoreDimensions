#include "CustomDimensionManager.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/core/dimension/CustomDimensionConfig.h"

#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"

#include "mc/world/level/DimensionManager.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/dimension/VanillaDimensions.h"

namespace more_dimensions {

auto& loggerMoreDimMag = MoreDimension::getInstance().getSelf().getLogger();

LL_TYPE_INSTANCE_HOOK(
    CheckCustomDimensionId,
    ll::memory::HookPriority::Normal,
    DimensionManager,
    &DimensionManager::serverLoadDimensionNameIdStoreTable,
    void,
    LevelStorage const& levelStorage
){
    origin(levelStorage);
}

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

DimensionType CustomDimensionManager::getDimensionIdFromName(std::string const& dimName) {
    return VanillaDimensions::fromString(dimName);
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

    Impl::DimensionInfo info;
    bool                newDim = false;
    if (impl->customDimensionMap.contains(dimName)) {
        info = impl->customDimensionMap.at(dimName);
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
        impl->customDimensionMap.emplace(dimName, info);
        CustomDimensionConfig::getConfig().dimensionList.emplace(
            dimName,
            CustomDimensionConfig::Config::Info{info.nbt.toSnbt(SnbtFormat::Minimize)}
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
