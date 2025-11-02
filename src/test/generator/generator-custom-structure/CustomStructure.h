#pragma once

#include "mc/deps/core/utility/NonOwnerPointer.h"

#include "gsl/pointers"

class FeatureRegistry;
class JigsawStructureRegistry;
class WorkerPool;
class StructureManager;
class BaseGameVersion;
class Experiments;

namespace custom_structure {

namespace CustomJigsawStructure {

// 总注册初始化函数
void initialize(
    Bedrock::NotNullNonOwnerPtr<::StructureManager> manager,
    FeatureRegistry&                                featureRegistry,
    JigsawStructureRegistry&                        registry,
    BaseGameVersion const&                          baseGameVersion,
    Experiments const&                              experiments
);

} // namespace CustomJigsawStructure

namespace CustomJigsawStructureBlockRules {

    // registry block rules
void initialize(JigsawStructureRegistry& registry);

}

namespace CustomJigsawStructureBlockTagRules {

    // registry block tag rules
void initialize(JigsawStructureRegistry& registry);

}

namespace CustomJigsawStructureElements {

void initialize(
    gsl::not_null<Bedrock::NonOwnerPointer<StructureManager>> manager,
    FeatureRegistry&                                          featureRegistry,
    JigsawStructureRegistry&                                  jigsawRegistry
);

}

} // namespace custom_structure
