#pragma once

#include "mc/world/level/levelgen/structure/StructureStart.h"


class CustomStructureStart : public StructureStart {
public:
    CustomStructureStart() = default;
    std::string_view getStructureName() const override { return "mike:basic_dungeon"; }
};
