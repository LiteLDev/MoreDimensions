#include "CustomStructurePiece.h"

#include "mc/world/level/BlockPos.h"
#include "mc/world/level/block/VanillaBlockTypeIds.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/structure/JigsawPlacement.h"
#include "mc/world/level/levelgen/structure/registry/JigsawStructureRegistry.h"
#include "mc/world/level/levelgen/structure/structurepools/StructureTemplatePool.h"
#include "mc/world/level/levelgen/structure/structurepools/alias/PoolAliasBinding.h"
#include "mc/world/level/levelgen/v1/AdjustmentEffect.h"
#include <memory>


namespace custom_structure {

int CustomStructurePiece::
    generateHeightAtPosition(BlockPos const&, Dimension& dim, BlockVolume&, std::unordered_map<ChunkPos, std::unique_ptr<std::vector<short>>>&)
        const {
    return dim.mSeaLevel + 1;
};

Block const* CustomStructurePiece::getSupportBlock(::BlockSource&, ::BlockPos const&, ::Block const&) const {
    return &BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::Netherrack(), 1);
};

Block const& CustomStructurePiece::getBeardStabilizeBlock(::Block const&) const {
    return BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::Netherrack(), 1);
};

AdjustmentEffect CustomStructurePiece::getTerrainAdjustmentEffect() const { return AdjustmentEffect::BeardAndShave; };

CustomStructurePiece::CustomStructurePiece(
    ::StructurePoolElement const& element,
    ::BlockPos                    position,
    ::Rotation                    rotation,
    int                           genDepth,
    ::JigsawJunction&             junction,
    ::BoundingBox const&          box,
    ::BlockPos                    refPos
)
: PoolElementStructurePiece(element, position, rotation, genDepth, junction, box, refPos){};

void CustomStructurePiece::addPieces(
    BlockPos                                      position,
    std::vector<std::unique_ptr<StructurePiece>>& pieces,
    Random&                                       random,
    JigsawStructureRegistry&                      pools,
    VanillaBiomeTypes                             biomeType,
    Dimension&                                    dimension
) {
    auto templates = pools.lookupByName("mike:21room");
    // templates.getRandomTemplate(random);

    auto lambda = [](StructurePoolElement const& element,
                     ::BlockPos const&           pos,
                     ::Rotation const&           rot,
                     int                         unk,
                     ::JigsawJunction&           jigction,
                     ::BoundingBox const&        bb,
                     ::BlockPos const&           pos1) {
        return std::make_unique<CustomStructurePiece>(element, pos, rot, unk, jigction, bb, pos1);
    };
    JigsawPlacement place(15, 80, pieces, lambda, random, pools, dimension);

    place.addPieces(*templates->getRandomTemplate(random), position, Rotation::Rotate90, "", {});
}

} // namespace custom_structure

PoolAliasBinding::PoolAliasLookup::PoolAliasLookup()=default;