#include "ll/api/event/EventBus.h"
#include "ll/api/event/server/ServerStartedEvent.h"
#include "mc/network/packet/DimensionDataPacket.h"
#include "more_dimensions/api/dimension/CustomDimensionManager.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"

static bool reg = [] {
    using namespace ll::event;
    EventBus::getInstance().emplaceListener<ServerStartedEvent>([](ServerStartedEvent&) {
        ll::dimension::CustomDimensionManager::getInstance().addDimension<ll::dimension::SimpleCustomDimension>(
            "testNewDimension"
        );
        ll::dimension::CustomDimensionManager::getInstance()
            .addDimension<ll::dimension::SimpleCustomDimension>("testNewFlatDimension", 345, GeneratorType::Flat);
        ll::dimension::CustomDimensionManager::getInstance()
            .addDimension<ll::dimension::SimpleCustomDimension>("testNewNetherDimension", 345, GeneratorType::Nether);
        ll::dimension::CustomDimensionManager::getInstance()
            .addDimension<ll::dimension::SimpleCustomDimension>("testNewTheEndDimension", 345, GeneratorType::TheEnd);
        ll::dimension::CustomDimensionManager::getInstance()
            .addDimension<ll::dimension::SimpleCustomDimension>("testNewVoidDimension", 345, GeneratorType::Void);
    });
    return true;
}();