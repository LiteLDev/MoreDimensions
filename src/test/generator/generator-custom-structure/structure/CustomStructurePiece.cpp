#include "CustomStructurePiece.h"

#include "mc/util/Random.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/block/VanillaBlockTypeIds.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/structure/BoundingBox.h"
#include "mc/world/level/levelgen/structure/JigsawPlacement.h"
#include "mc/world/level/levelgen/structure/registry/JigsawStructureRegistry.h"
#include "mc/world/level/levelgen/structure/structurepools/StructurePoolElement.h"
#include "mc/world/level/levelgen/structure/structurepools/StructureTemplatePool.h"
#include "mc/world/level/levelgen/v1/AdjustmentEffect.h"

#include <memory>

StructureTemplatePool::StructureTemplatePool() = default;

namespace custom_structure {

int CustomStructurePiece::generateHeightAtPosition(
    BlockPos const&,
    Dimension& dim,
    BlockVolume&,
    std::unordered_map<ChunkPos, std::unique_ptr<std::vector<short>>>&
) const {
    return dim.mSeaLevel + 100;
};

Block const* CustomStructurePiece::getSupportBlock(::BlockSource&, ::BlockPos const&, ::Block const&) const {
    return &BlockTypeRegistry::get().getDefaultBlockState(VanillaBlockTypeIds::GrassBlock());
};

Block const& CustomStructurePiece::getBeardStabilizeBlock(::Block const&) const {
    return BlockTypeRegistry::get().getDefaultBlockState(VanillaBlockTypeIds::GrassBlock());
};

AdjustmentEffect CustomStructurePiece::getTerrainAdjustmentEffect() const { return AdjustmentEffect::Beard; };

CustomStructurePiece::CustomStructurePiece(
    ::StructurePoolElement const& element,
    ::BlockPos                    position,
    ::Rotation                    rotation,
    int                           genDepth,
    ::JigsawJunction&             junction,
    ::BoundingBox const&          box,
    ::BlockPos                    refPos
)
: PoolElementStructurePiece(element, position, rotation, genDepth, junction, box, refPos) {};

void CustomStructurePiece::addPieces(
    BlockPos                                      position,
    std::vector<std::unique_ptr<StructurePiece>>& pieces,
    Random&                                       random,
    JigsawStructureRegistry const&                pools,
    VanillaBiomeTypes                             biomeType,
    Dimension&                                    dimension
) {
    auto templates = pools.lookupByName("mike:ns7x4");
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

    auto index = random.nextInt(0, templates->mTemplates->size() - 1);
    place.addPieces(*templates->mTemplates->at(index), position, static_cast<Rotation>(random.nextInt(4)), "", {});
}

} // namespace custom_structure
