#include "ll/api/service/Bedrock.h"
#include "test/generator/flat-gen-village/FlatVillageDimension.h"
// #include "test/generator/generator-custom-structure/dimension/CustomStructureDimension.h"
#include "test/generator/generator-terrain/NxnBorderTerrainDimension.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/event/command/ServerCommandRegisterEvent.h"
#include "ll/api/event/server/ServerStartedEvent.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/memory/Signature.h"

#include "more_dimensions/api/dimension/CustomDimensionManager.h"

#include "mc/network/packet/DimensionDataPacketPayload.h"
#include "mc/world/level/DimensionManager.h"
#include "mc/world/level/Level.h"

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

// Since 26.51 levilamina's headers no longer declare
// ServerInstanceEventCoordinator::sendServerInitializeEnd (the function still exists in the
// binary, but without the declaration there is no member function pointer to hook), so use
// LeviLamina's own ServerStartedEvent instead; it fires at an equivalent point.
static auto _registryTestDimensionListener =
    ll::event::EventBus::getInstance().emplaceListener<ll::event::ServerStartedEvent>(
        [](ll::event::ServerStartedEvent&) { registryTestDimension(); }
    );

using namespace ll::literals::memory_literals;

// In 26.51 DimensionDataPacket::$ctor became two overloads, () and (DimensionDataPacketPayload),
// so no member function pointer can be deduced. Hook the layer where the dimension data is
// actually moved in instead: the DimensionDataPacketPayload constructor.
// LeviLamina 26.60 no longer exports DimensionDataPacketPayload::$ctor, so it is found by signature
// (??0DimensionDataPacketPayload@@QEAA@$$QEAVDimensionDefinitionGroup@@@Z; the same bytes on the client
// 1.26.60.5 and the server 1.26.60).
LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckCustomDimensionReg,
    ll::memory::HookPriority::Normal,
    DimensionDataPacketPayload,
    "56 57 53 48 83 EC 20 48 89 D7 48 89 CE 0F 57 C0 0F 11 01 48 8B 0D"_sig,
    void*,
    DimensionDefinitionGroup&& dimensionDefinitionGroup
) {
    for (const auto& item : *dimensionDefinitionGroup.mDimensionDefinitions) {
        std::cout << "name: " << item.first << "Type: " << item.second.mDimensionType->mValue << std::endl;
    }
    return origin(std::move(dimensionDefinitionGroup));
}