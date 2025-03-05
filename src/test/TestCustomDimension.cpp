#include "test/generator/flat-gen-village/FlatVillageDimension.h"
// #include "test/generator/generator-custom-structure/dimension/CustomStructureDimension.h"
#include "test/generator/generator-terrain/NxnBorderTerrainDimension.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/event/server/ServerStartedEvent.h"

#include "more_dimensions/api/dimension/CustomDimensionManager.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"

static bool reg = [] {
    using namespace ll::event;
    EventBus::getInstance().emplaceListener<ServerStartedEvent>([](ServerStartedEvent&) {
        // simplate dimension test
        // vanilla overworld type dimension test
        more_dimensions::CustomDimensionManager::getInstance().addDimension<more_dimensions::SimpleCustomDimension>(
            "testNewDimension"
        );

        // vanilla flat type dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewFlatDimension", 345, GeneratorType::Flat);

        // vanilla nether type dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewNetherDimension", 345, GeneratorType::Nether);

        // vanilla the end type dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewTheEndDimension", 345, GeneratorType::TheEnd);

        // vanilla void dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<more_dimensions::SimpleCustomDimension>("testNewVoidDimension", 345, GeneratorType::Void);

        // custom diomension test
        // flat type generator village dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<flat_village_dimension::FlatVillageDimension>("testFlatVillage");

        // flat type custom terrain dimension test
        more_dimensions::CustomDimensionManager::getInstance()
            .addDimension<nxn_border_terrain::NxnBorderTerrainDimension>("testFlatTerrain", 5);

        // // flat type custom structure dimension test
        // more_dimensions::CustomDimensionManager::getInstance()
        //     .addDimension<custom_structure_dimension::CustomStructureDimension>("testCustomStructure");
    });
    return true;
}();


// #include "ll/api/memory/Hook.h"
// #include "mc/world/level/DimensionManager.h"


// LL_AUTO_TYPE_INSTANCE_HOOK(
//     GetOrCreateDimension,
//     HookPriority::Normal,
//     DimensionManager,
//     &DimensionManager::getOrCreateDimension,
//     WeakRef<Dimension>,
//     DimensionType dim
// ) {
//     std::cout << "getOrCreateDimension->" << dim.id << std::endl;
//     return origin(dim);
// };

// #include "mc/server/commands/standard/TeleportCommand.h"
// #include "mc/server/commands/standard/TeleportTarget.h"
// #include "mc/util/rotation_command_utils/RotationData.h"
// #include "mc/world/level/dimension/VanillaDimensions.h"
// LL_AUTO_TYPE_STATIC_HOOK(
//     TeleportCommandTest,
//     HookPriority::Normal,
//     TeleportCommand,
//     &TeleportCommand::computeTarget,
//     TeleportTarget,
//     ::Actor&                                                     victim,
//     ::Vec3                                                       destination,
//     ::Vec3*                                                      facePosition,
//     ::DimensionType                                              destinationDimension,
//     ::std::optional<::RotationCommandUtils::RotationData> const& rotationData,
//     int                                                          commandVersion
// ) {
//     for(auto& item:VanillaDimensions::DimensionMap().mLeft) {
//         std::cout<<"Dimension name: "<<item.second<<" Id: "<<item.first<<std::endl;
//     }
//     for(auto& item:VanillaDimensions::DimensionMap().mRight) {
//         std::cout<<"Dimension name: "<<item.first<<" Id: "<<item.second<<std::endl;
//     }
//     std::cout << "computeTarget->" << destinationDimension << std::endl;
//     return origin(victim, destination, facePosition, destinationDimension, rotationData, commandVersion);
// };

//
// LL_AUTO_TYPE_INSTANCE_HOOK(
//     DimensonFactory123,
//     HookPriority::Normal,
//     DimensionFactory,
//     "?create@DimensionFactory@@UEBA?AV?$OwnerPtr@VDimension@@@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
//     OwnerPtr<Dimension>,
//     std::string const &name) {
//     std::cout<<"Dimension creative->"<<name<<std::endl;;
//     return origin(name);
// };
//
// #include "ll/api/command/CommandHandle.h"
// #include "ll/api/command/CommandRegistrar.h"
// #include "mc/server/commands/CommandOutput.h"
// #include "mc/server/commands/ServerCommands.h"
// #include "ll/api/service/Bedrock.h"
// #include "mc/world/level/Level.h"
// #include "mc/world/level/dimension/DimensionFactory.h"
// #include "mc/world/level/dimension/VanillaDimensions.h"
//
// #include "mc/world/events/ServerInstanceEventCoordinator.h"
//
// struct ParamTest {
//     int      p1;
// };
//
// LL_AUTO_TYPE_INSTANCE_HOOK(
//     registerBuiltinCommands,
//     ll::memory::HookPriority::Normal,
//     ServerInstanceEventCoordinator,
//     &ServerInstanceEventCoordinator::sendServerThreadStarted,
//     void,
//     ::ServerInstance& ins
//) {
//     origin(ins);
//
//     auto&       cmd    = ll::command::CommandRegistrar::getInstance().getOrCreateCommand("t", "test tttttt");
//     static auto lambda = [](CommandOrigin const&, CommandOutput& output, ParamTest const& param) {
//         output.success("p1: {}", param.p1);
//         auto dim = ll::service::getLevel()->getOrCreateDimension(param.p1);
//         if (dim.expired()) {
//             std::cout<<"他宝贝的，销毁了"<<std::endl;
//         } else {
//             auto dim_ptr = dim.lock();
//             std::cout<<"这是正常的:"<<dim_ptr->mName<<std::endl;
//         }
//         auto& dimM = ll::service::getLevel()->getDimensionManager();
//         for (auto& item: dimM.mDimensions) {
//             std::cout<<"Dimension have->" <<item.first.id<<std::endl;
//         };
//         for (auto item: VanillaDimensions::DimensionMap.mLeft) {
//             std::cout<< "Dimension Factory->"<<item.first<<",name:"<<item.second<<std::endl;
//             std::cout<< "Dimension Factory->"<<item.first<<",get
//             name:"<<VanillaDimensions::toString(item.first)<<std::endl;
//         }
//     };
//     cmd.overload<ParamTest>()
//         .required("p1")
//         .execute(lambda);
// }

// #include "mc/world/level/LoadingScreenIdManager.h"
// #include "ll/api/memory/Hook.h"

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     ScreenIdHookTest,
//     ll::memory::HookPriority::Normal,
//     LoadingScreenIdManager,
//     &LoadingScreenIdManager::getNextLoadingScreenId,
//     NewType<::std::optional<uint>>) {
//     auto result =  origin();
//     if (result.mValue.has_value()){
//         std::cout<<"Test screedId: "<<result.mValue.value()<<std::endl;
//     }else {
//         std::cout<<"Test screedId: Null"<<std::endl;
//     }
//     return result;
// }