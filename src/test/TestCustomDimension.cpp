#include "ll/api/service/Bedrock.h"
#include "test/generator/flat-gen-village/FlatVillageDimension.h"
// #include "test/generator/generator-custom-structure/dimension/CustomStructureDimension.h"
#include "test/generator/generator-terrain/NxnBorderTerrainDimension.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/event/command/ServerCommandRegisterEvent.h"
#include "ll/api/memory/Hook.h"

#include "more_dimensions/api/dimension/CustomDimensionManager.h"

#include "mc/world/level/DimensionManager.h"
#include "mc/world/level/Level.h"
#include "mc/world/events/ServerInstanceEventCoordinator.h"
#include "mc/network/packet/DimensionDataPacket.h"

void registryTestDimension() {
    // simplate dimension test
    // vanilla overworld type dimension test
    more_dimensions::CustomDimensionManager::getInstance().addSimpleDimension("custom_dim:new_overworld_type");

    // vanilla flat type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addSimpleDimension("custom_dim:new_flat_type", 345, GeneratorType::Flat);

    // vanilla nether type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addSimpleDimension("custom_dim:new_nether_type", 345, GeneratorType::Nether);

    // vanilla the end type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addSimpleDimension("custom_dim:new_theend_type", 345, GeneratorType::TheEnd);

    // vanilla void dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addSimpleDimension("custom_dim:new_void_type", 345, GeneratorType::Void);

    // custom diomension test
    // flat type generator village dimension test
    more_dimensions::CustomDimensionManager::getInstance().addDimension<flat_village_dimension::FlatVillageDimension>(
        "custom_dim:new_flat_village_type"
    );

    // flat type custom terrain dimension test
    more_dimensions::CustomDimensionManager::getInstance().addDimension<nxn_border_terrain::NxnBorderTerrainDimension>(
        "custom_dim:new_flat_custom_terrain",
        5
    );

    // flat type custom structure dimension test
    // more_dimensions::CustomDimensionManager::getInstance()
    //     .addDimension<custom_structure_dimension::CustomStructureDimension>("test:testCustomStructure");
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckCustomDimensionId,
    ll::memory::HookPriority::Normal,
    ServerInstanceEventCoordinator,
    &ServerInstanceEventCoordinator::sendServerInitializeEnd,
    void,
    ServerInstance& ins
) {
    origin(ins);
    registryTestDimension();
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckCustomDimensionReg,
    ll::memory::HookPriority::Normal,
    DimensionDataPacket,
    &DimensionDataPacket::$ctor,
    void*,
    DimensionDefinitionGroup const& dimensionDefinitionGroup
) {
    for (const auto& item: *dimensionDefinitionGroup.mDimensionDefinitions) {
        std::cout<<"name: "<<item.first << "Type: "<<item.second.mDimensionType->value()<<std::endl;
    }
    return origin(dimensionDefinitionGroup);
}