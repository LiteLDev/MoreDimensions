#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/world/level/levelgen/structure/Projection.h"

// auto generated forward declare list
// clang-format off
class StructurePoolElement;
struct StructureTemplateRegistrationContext;
// clang-format on

struct WeightedStructureTemplateRegistration {
public:
    // member variables
    // NOLINTBEGIN
    std::function<::StructurePoolElement const&(::StructureTemplateRegistrationContext, ::Projection)> mElementFactory;
    int                                                                                                mWeight;
    // NOLINTEND
};
