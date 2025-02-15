#pragma once

#include "mc/deps/core/utility/NonOwnerPointer.h"

#include "gsl/pointers"

class FeatureRegistry;
class JigsawStructureRegistry;
class WorkerPool;
class StructureManager;

namespace custom_structure {

namespace CustomJigsawStructure {

// 总注册初始化函数
void initialize(
    Bedrock::NotNullNonOwnerPtr<::StructureManager> manager,
    FeatureRegistry&                                featureRegistry,
    JigsawStructureRegistry&                        registry
);

} // namespace CustomJigsawStructure

namespace CustomJigsawStructureBlockRules {

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
