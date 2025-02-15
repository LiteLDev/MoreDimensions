#pragma once

#include "mc/world/level/biome/components/vanilla/VanillaBiomeTypes.h"
#include "mc/world/level/levelgen/structure/PoolElementStructurePiece.h"


class JigsawStructureRegistry;

namespace custom_structure {

class CustomStructurePiece : public PoolElementStructurePiece {
public:
    virtual int
    generateHeightAtPosition(::BlockPos const&, ::Dimension&, ::BlockVolume&, ::std::unordered_map<::ChunkPos, ::std::unique_ptr<::std::vector<short>>>&)
        const;

    virtual Block const* getSupportBlock(::BlockSource&, ::BlockPos const&, ::Block const&) const;

    virtual Block const& getBeardStabilizeBlock(::Block const&) const;

    virtual AdjustmentEffect getTerrainAdjustmentEffect() const;

    CustomStructurePiece(
        ::StructurePoolElement const& element,
        ::BlockPos                    position,
        ::Rotation                    rotation,
        int                           genDepth,
        ::JigsawJunction&             junction,
        ::BoundingBox const&          box,
        ::BlockPos                    refPos
    );

    static void addPieces(
        BlockPos                                      position,
        std::vector<std::unique_ptr<StructurePiece>>& pieces,
        Random&                                       random,
        JigsawStructureRegistry&                      pools,
        VanillaBiomeTypes                             biomeType,
        Dimension&                                    dimension
    );
};

} // namespace custom_structure
