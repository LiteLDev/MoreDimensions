#include "ll/api/event/EventBus.h"
#include "ll/api/event/server/ServerStartedEvent.h"
#include "more_dimensions/api/dimension/CustomDimensionManager.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"

static bool reg = [] {
    using namespace ll::event;
    EventBus::getInstance().emplaceListener<ServerStartedEvent>([](ServerStartedEvent&) {
        more_dimensions::CustomDimensionManager::getInstance().addDimension<more_dimensions::SimpleCustomDimension>(
            "testNewDimension"
        );
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewFlatDimension", 345, GeneratorType::Flat);
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewNetherDimension", 345, GeneratorType::Nether);
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewTheEndDimension", 345, GeneratorType::TheEnd);
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewVoidDimension", 345, GeneratorType::Void);
    });
    return true;
}();