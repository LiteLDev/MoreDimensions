#pragma once

#include "mc/world/level/levelgen/structure/StructureStart.h"


class CustomStructureStart : public StructureStart {
public:
    CustomStructureStart() = default;
    virtual std::string_view getStructureName() const { return "mike:basic_dungeon"; };
};
