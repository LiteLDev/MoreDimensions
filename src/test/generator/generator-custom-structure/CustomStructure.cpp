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
    auto& jigsawBlockRulesRegistry = registry.mJigsawBlockRulesRegistry;

    // input_predicate block
    auto& stoneBricks       = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneBricks());

    // output_state blocks
    auto& mossyStoneBricks = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::MossyStoneBricks());
    auto& crackedStoneBricks = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::CrackedStoneBricks());
    auto& cobblestone = BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::Cobblestone());

    // input_predicate
    std::unique_ptr<IStructurePoolBlockPredicate> sourceBlock_pro1 =
        std::make_unique<StructurePoolBlockPredicateBlockMatchRandom>(stoneBricks, 0.2);
    std::unique_ptr<IStructurePoolBlockPredicate> sourceBlock_pro2 =
        std::make_unique<StructurePoolBlockPredicateBlockMatchRandom>(stoneBricks, 0.1);

    std::unique_ptr<IStructurePoolBlockPredicate> targetBlock =
        std::make_unique<StructurePoolBlockPredicateAlwaysTrue>();

    auto blockRule_1 =
        std::make_unique<StructurePoolBlockRule>(std::move(sourceBlock_pro1), std::move(targetBlock), &mossyStoneBricks);
    auto blockRule_2 =
        std::make_unique<StructurePoolBlockRule>(std::move(sourceBlock_pro1), std::move(targetBlock), &crackedStoneBricks);
    auto blockRule_3 =
        std::make_unique<StructurePoolBlockRule>(std::move(sourceBlock_pro2), std::move(targetBlock), &cobblestone);


    auto ruleList = std::make_unique<std::vector<std::unique_ptr<StructurePoolBlockRule>>>();

    ruleList->emplace_back(std::move(blockRule_1));
    ruleList->emplace_back(std::move(blockRule_2));
    ruleList->emplace_back(std::move(blockRule_3));

    jigsawBlockRulesRegistry->registerBlockRules("mike:stone_brick_replacement_jig", std::move(ruleList));

    // br::worldgen::processors::BlockRules::Test block_test;
    // block_test.mData = br::worldgen::processors::BlockRules::RandomBlockMatch(stoneBricks.mLegacyBlock.get(), 0.2);

    // auto blockRule_1 = br::worldgen::processors::RuleSet::from(block_test, mossyStoneBricks);
    // auto blockRule_2 = br::worldgen::processors::RuleSet::from(block_test, crackedStoneBricks);
    // auto blockRule_3 = br::worldgen::processors::RuleSet::from(block_test, cobblestone);

    // auto ruleList = br::worldgen::StructureProcessor::Rule({blockRule_1, blockRule_2, blockRule_3});
    // auto prot = br::worldgen::StructureProcessor::Protected(VanillaBlockTypeIds::RedstoneBlock());
    // jigsawBlockRulesRegistry->record("mike:stone_brick_replacement", {ruleList, prot});

}

void CustomJigsawStructureElements::initialize(
    gsl::not_null<Bedrock::NonOwnerPointer<StructureManager>> manager,
    FeatureRegistry&                                          featureRegistry,
    JigsawStructureRegistry&                                  jigsawRegistry
) {
    auto& jigsawBlockRulesRegistry       = jigsawRegistry.mJigsawBlockRulesRegistry;
    auto& jigsawStructureElementRegistry = jigsawRegistry.mJigsawElementRegistry;
    auto  ruleList                       = jigsawBlockRulesRegistry->lookupByName("mike:stone_brick_replacement");

    // 每一个结构nbt文件都得这样注册进来，多个nbt结构文件的可以使用同一个Block Rule
    // mike:5x5int
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:5x5intb",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/beds5x5int",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:5x5intc",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/chestcarpet5x5int",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:5x5intk",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/kitchen5x5int",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );

    //mike:ew7x4
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:ew7x4h",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/ewhall",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:ew7x4r",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/21room",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );

    //mike:ns7x4
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:ns7x4h",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/nshall",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:ns7x4r",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/21room",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );

    //mike:ewcap
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:ewcap",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/ewcap",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
        )
    );

    //mike:nacap
    jigsawStructureElementRegistry->registerStructureElement(
        "mike:nscap",
        std::make_unique<StructurePoolElement>(
            manager,
            "custom/nscap",
            ruleList,
            nullptr,
            nullptr,
            Projection::TerrainMatching,
            PostProcessSettings::All
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
    auto& jigsawStructureElementRegistry = registry.mJigsawElementRegistry;

    std::vector<std::pair<StructurePoolElement const*, int>> templates_5x5{
        {jigsawStructureElementRegistry->lookupByName("mike:5x5intb"), 4},
        {jigsawStructureElementRegistry->lookupByName("mike:5x5intc"), 1},
        {jigsawStructureElementRegistry->lookupByName("mike:5x5intk"), 1}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_ew{
        {jigsawStructureElementRegistry->lookupByName("mike:ew7x4h"), 4},
        {jigsawStructureElementRegistry->lookupByName("mike:ew7x4r"), 1}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_ns{
        {jigsawStructureElementRegistry->lookupByName("mike:ns7x4h"), 4},
        {jigsawStructureElementRegistry->lookupByName("mike:ns7x4r"), 1}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_ewcap{
        {jigsawStructureElementRegistry->lookupByName("mike:ewcap"), 4}
    };
    std::vector<std::pair<StructurePoolElement const*, int>> templates_nscap{
        {jigsawStructureElementRegistry->lookupByName("mike:nscap"), 4}
    };

    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:5x5int", "empty", templates_5x5));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:ew7x4", "mike:ewcap", templates_ew));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:ns7x4", "mike:nscap", templates_ns));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:ewcap", "empty", templates_ewcap));
    registry.registerPool(std::make_unique<StructureTemplatePool>("mike:nscap", "empty", templates_nscap));
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