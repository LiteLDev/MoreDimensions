#include "CustomStructure.h"

#include "ll/api/memory/Hook.h"

#include "mc/deps/core/threading/WorkerPool.h"
#include "mc/resources/BaseGameVersion.h"
#include "mc/world/level/block/VanillaBlockTypeIds.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/levelgen/feature/registry/FeatureRegistry.h"
#include "mc/world/level/levelgen/structure/Projection.h"
#include "mc/world/level/levelgen/structure/StructureManager.h"
#include "mc/world/level/levelgen/structure/registry/JigsawStructureBlockRulesRegistry.h"
#include "mc/world/level/levelgen/structure/registry/JigsawStructureElementRegistry.h"
#include "mc/world/level/levelgen/structure/registry/JigsawStructureRegistry.h"
#include "mc/world/level/levelgen/structure/registry/StructurePools.h"
#include "mc/world/level/levelgen/structure/structurepools/StructurePoolBlockPredicateAlwaysTrue.h"
#include "test/mc/StructurePoolBlockPredicateBlockMatchRandom.h"
#include "test/mc/StructurePoolBlockRule.h"
#include "mc/world/level/levelgen/structure/structurepools/StructurePoolElement.h"
#include "mc/world/level/levelgen/structure/structurepools/StructureTemplatePool.h"
#include "mc/world/level/storage/Experiments.h"

#include <memory>

namespace custom_structure {

void CustomJigsawStructureBlockRules::initialize(JigsawStructureRegistry& registry) {
    auto& jigsawBlockRulesRegistry = registry.mUnk807a76.as<JigsawStructureBlockRulesRegistry>();
    // auto& DefaultBlockState =
    // BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::PolishedBlackstoneBricks(), 1);
    auto& resultBlock = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::RedstoneBlock(), 1);
    auto& block       = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::Blackstone(), 1);

    std::unique_ptr<IStructurePoolBlockPredicate> sourceBlock =
        std::make_unique<StructurePoolBlockPredicateBlockMatchRandom>(block, 0.3);
    std::unique_ptr<IStructurePoolBlockPredicate> targetBlock =
        std::make_unique<StructurePoolBlockPredicateAlwaysTrue>();

    auto blockRule =
        std::make_unique<StructurePoolBlockRule>(std::move(sourceBlock), std::move(targetBlock), &resultBlock);
    auto ruleList = std::make_unique<std::vector<std::unique_ptr<StructurePoolBlockRule>>>();

    ruleList->push_back(std::move(blockRule));

    jigsawBlockRulesRegistry.registerBlockRules("custom:custom_structure_block_rule", std::move(ruleList));
}

void CustomJigsawStructureElements::initialize(
    gsl::not_null<Bedrock::NonOwnerPointer<StructureManager>> manager,
    FeatureRegistry&                                          featureRegistry,
    JigsawStructureRegistry&                                  jigsawRegistry
) {
    auto& jigsawBlockRulesRegistry       = jigsawRegistry.mUnk807a76.as<JigsawStructureBlockRulesRegistry>();
    auto& jigsawStructureElementRegistry = jigsawRegistry.mUnk8230cc.as<JigsawStructureElementRegistry>();
    auto  ruleList                       = jigsawBlockRulesRegistry.lookupByName("custom:custom_structure_block_rule");

    // 每一个结构nbt文件都得这样注册进来，多个nbt结构文件的可以使用同一个Block Rule
    jigsawStructureElementRegistry.registerStructureElement(
        "mike:21room",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/21room",
            ruleList,
            nullptr,
            nullptr,
            Projection::Rigid,
            PostProcessSettings::None
        )
    );

    jigsawStructureElementRegistry.registerStructureElement(
        "mike:ew7x4",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/ewhall",
            ruleList,
            nullptr,
            nullptr,
            Projection::Rigid,
            PostProcessSettings::None
        )
    );

    jigsawStructureElementRegistry.registerStructureElement(
        "mike:ns7x4",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/nshall",
            ruleList,
            nullptr,
            nullptr,
            Projection::Rigid,
            PostProcessSettings::None
        )
    );
}

void CustomJigsawStructure::initialize(
    Bedrock::NotNullNonOwnerPtr<::StructureManager> manager,
    FeatureRegistry&                                featureRegistry,
    JigsawStructureRegistry&                        registry
) {
    CustomJigsawStructureBlockRules::initialize(registry);
    CustomJigsawStructureElements::initialize(manager, featureRegistry, registry);
    auto& jigsawStructureElementRegistry = registry.mUnk8230cc.as<JigsawStructureElementRegistry>();

    std::vector<std::pair<StructurePoolElement const*, int>> templates_room{
        {jigsawStructureElementRegistry.lookupByName("mike:21room"), 1}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_ew{
        {jigsawStructureElementRegistry.lookupByName("mike:ew7x4"), 1}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_ns{
        {jigsawStructureElementRegistry.lookupByName("mike:ns7x4"), 1}
    };

    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:21room", "empty", templates_room));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:ew7x4", "empty", templates_ew));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:ns7x4", "empty", templates_ns));
}

} // namespace custom_structure

LL_AUTO_TYPE_STATIC_HOOK(
    InitStructure,
    HookPriority::Normal,
    br::worldgen::StructurePools,
    br::worldgen::StructurePools::bootstrap,
    void,
    Bedrock::NotNullNonOwnerPtr<StructureManager> structureManager,
    FeatureRegistry&                              featureRegistry,
    JigsawStructureRegistry&                      jigsawStructureRegistry,
    BaseGameVersion const&                        baseGameVersion,
    Experiments const&                            experiments
) {
    origin(structureManager, featureRegistry, jigsawStructureRegistry, baseGameVersion, experiments);
    custom_structure::CustomJigsawStructure::initialize(structureManager, featureRegistry, jigsawStructureRegistry);
    return;
};