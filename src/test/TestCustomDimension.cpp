#include "ll/api/service/TargetedBedrock.h"
#include "test/generator/flat-gen-village/FlatVillageDimension.h"
// #include "test/generator/generator-custom-structure/dimension/CustomStructureDimension.h"
#include "test/generator/generator-terrain/NxnBorderTerrainDimension.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/api/dimension/CustomDimensionManager.h"
#include "more_dimensions/api/dimension/SimpleCustomDimension.h"

auto& logger = more_dimensions::MoreDimension::getInstance().getSelf().getLogger();

void registryTestDimensions(bool isClientSide) {
    // simplate dimension test
    // vanilla overworld type dimension test
    more_dimensions::CustomDimensionManager::getInstance().addDimension<more_dimensions::SimpleCustomDimension>(
        "testNewDimension",
        isClientSide
    );

    // vanilla flat type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addDimension<more_dimensions::SimpleCustomDimension>("testNewFlatDimension", isClientSide, 345, GeneratorType::Flat);

    // vanilla nether type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addDimension<more_dimensions::SimpleCustomDimension>("testNewNetherDimension", isClientSide, 345, GeneratorType::Nether);

    // vanilla the end type dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addDimension<more_dimensions::SimpleCustomDimension>("testNewTheEndDimension", isClientSide, 345, GeneratorType::TheEnd);

    // vanilla void dimension test
    more_dimensions::CustomDimensionManager::getInstance()
        .addDimension<more_dimensions::SimpleCustomDimension>("testNewVoidDimension", isClientSide, 345, GeneratorType::Void);

    // custom diomension test
    // flat type generator village dimension test
    more_dimensions::CustomDimensionManager::getInstance().addDimension<flat_village_dimension::FlatVillageDimension>(
        "testFlatVillage",
        isClientSide
    );

    // flat type custom terrain dimension test
    more_dimensions::CustomDimensionManager::getInstance().addDimension<nxn_border_terrain::NxnBorderTerrainDimension>(
        "testFlatTerrain",
        isClientSide,
        5
    );

    // flat type custom structure dimension test
    // more_dimensions::CustomDimensionManager::getInstance()
    //     .addDimension<custom_structure_dimension::CustomStructureDimension>("testCustomStructure", isClientSide);
}

#ifdef LL_PLAT_C

#include "ll/api/event/EventBus.h"
#include "ll/api/event/client/ClientStartJoinLevelEvent.h"
#include "ll/api/event/client/ClientJoinLevelEvent.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/dimension/VanillaDimensionFactory.h"
#include "mc/world/level/Level.h"

static bool reg = [] {
    using namespace ll::event;
    // EventBus::getInstance().emplaceListener<ClientJoinLevelEvent>([](ClientJoinLevelEvent&) { registryTestDimensions(true); });
    EventBus::getInstance().emplaceListener<ClientStartJoinLevelEvent>([](ClientStartJoinLevelEvent&) { registryTestDimensions(false); });
    return true;
}();

#else

#include "ll/api/event/EventBus.h"
#include "ll/api/event/server/ServerStartedEvent.h"

static bool reg = [] {
    using namespace ll::event;
    EventBus::getInstance().emplaceListener<ServerStartedEvent>([](ServerStartedEvent&) { registryTestDimensions(); });
    return true;
}();

#endif

// #include "mc/client/network/LegacyClientNetworkHandler.h"
// #include "mc/world/level/Level.h"

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     LegacyHandleHook,
//     ll::memory::HookPriority::Normal,
//     LegacyClientNetworkHandler,
//     &LegacyClientNetworkHandler::$handle,
//     void,
//     NetworkIdentifier const& source,
//     StartGamePacket const& packet
// ) {
//     origin(source, packet);
//     auto& dimensionMap = this->mUnk54d468.as<Bedrock::NonOwnerPointer<ILevel>>()->getDimensionFactory().mFactoryMap;
//     if (dimensionMap.empty()) {
//         logger.debug("Factory map is empty!");
//     } else {
//         logger.debug("Factory size is: {0}", dimensionMap.size());
//         for (auto& item: dimensionMap) {
//             logger.debug("dimName: {0} find!",item.first);
//         }
//     }
// }

// #include "ll/api/memory/Hook.h"
// #include "mc/world/level/DimensionManager.h"


LL_AUTO_TYPE_INSTANCE_HOOK(
    GetOrCreateDimension,
    HookPriority::Normal,
    Level,
    &Level::$getOrCreateDimension,
    WeakRef<Dimension>,
    DimensionType dim
) {
    logger.debug("CLient->{}, getOrCreateDimension->{}", isClientSide(), dim.id);
    auto result =  origin(dim);
    if (result.expired()) {
        logger.debug("Dimension creative fail!!! id:{}, registry dimensions", dim.id);
        registryTestDimensions(isClientSide());
        result = getOrCreateDimension(dim);
    }
    return result;
};

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
// #include "mc/world/actor/Actor.h"
// #include "mc/world/level/dimension/VanillaDimensions.h"

// struct ParamTest {
//     int      p1;
// };

// LL_AUTO_STATIC_HOOK(
//     RegisterTestCommands,
//     ll::memory::HookPriority::Normal,
//     &ServerCommands::setupStandardServer,
//     void,
//     Minecraft&         server,
//     std::string const& networkCommands,
//     std::string const& networkTestCommands,
//     PermissionsFile*   permissionsFile
// ) {
//     origin(server, networkCommands, networkTestCommands, permissionsFile);

//     auto&       cmd    = ll::command::CommandRegistrar::getInstance().getOrCreateCommand("tpme", "test tttttt");
//     static auto lambda = [](CommandOrigin const& origin, CommandOutput& output, ParamTest const& param) {
//         auto self = origin.getEntity();
//         Vec3 pos{1, 100, 1};
//         self->teleport(pos, param.p1);
//             output.success("Teleported {0} to {1} {2}",
//                 origin.getName(),
//                 VanillaDimensions::toString(param.p1),
//                 pos.toString()
//             );
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