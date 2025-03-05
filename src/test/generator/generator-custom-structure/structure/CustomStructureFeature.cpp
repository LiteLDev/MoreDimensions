#include "CustomStructureFeature.h"

#include "test/generator/generator-custom-structure/structure/CustomStructurePiece.h"
#include "test/generator/generator-custom-structure/structure/CustomStructureStart.h"


#include "mc/deps/core/string/HashedString.h"
#include "mc/world/level/ChunkPos.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/Biome.h"
#include "mc/world/level/biome/source/BiomeSource.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/structure/BoundingBox.h"
#include "mc/world/level/levelgen/structure/StructureStart.h"



#include <memory>

namespace custom_structure {
bool CustomStructureFeature::getNearestGeneratedFeature(
    ::Dimension&                           dimension,
    ::BiomeSource const&                   biomeSource,
    ::BlockPos const&                      origin,
    ::BlockPos&                            pos,
    ::IPreliminarySurfaceProvider const&   preliminarySurfaceLevel,
    bool                                   mustBeInNewChunks,
    ::std::optional<::HashedString> const& biomeTag
) {
    return false;
};

bool CustomStructureFeature::
    isFeatureChunk(::BiomeSource const&, ::Random&, ::ChunkPos const& cpos, uint, ::IPreliminarySurfaceProvider const&, ::Dimension const&) {
    if (cpos.x == 1 && cpos.z == 1) {
        return true;
    }
    return false;
};

std::unique_ptr<::StructureStart> CustomStructureFeature::
    createStructureStart(::Dimension& dim, ::BiomeSource const& biomeSource, ::Random& random, ::ChunkPos const& cpos, ::IPreliminarySurfaceProvider const&) {
    auto start = std::make_unique<CustomStructureStart>();
    CustomStructurePiece::addPieces(
        {cpos.x * 16, 0, cpos.z * 16},
        start->pieces,
        random,
        dim.mLevel.getJigsawStructureRegistry(),
        biomeSource.getBiome(cpos.x * 16, 1, cpos.z * 16)->getBiomeType(),
        dim
    );
    start->calculateBoundingBox();
    return std::move(start);
};

CustomStructureFeature::CustomStructureFeature(uint seed)
: StructureFeature(seed, HashedString("custom:custom_structure_test")) {}

} // namespace custom_structure
