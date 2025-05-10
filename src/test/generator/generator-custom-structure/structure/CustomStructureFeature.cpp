#include "CustomStructureFeature.h"

#include "test/generator/generator-custom-structure/structure/CustomStructurePiece.h"
#include "test/generator/generator-custom-structure/structure/CustomStructureStart.h"


#include "mc/deps/core/math/Random.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/util/Random.h"
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
    isFeatureChunk(::BiomeSource const&, ::Random& random, ::ChunkPos const& cpos, uint, ::IPreliminarySurfaceProvider const&, ::Dimension const&) {
    int  salt    = 0x13272F0;
    uint newSeed = mSeed + salt + 0xF1565BD5 * (cpos.z / mMaxSpacing) - 0x66C60AF8 * (cpos.x / mMaxSpacing);
    random.mRandom->mObject.mHaveNextNextGaussian   = false;
    random.mRandom->mObject.mSeed                   = newSeed;
    random.mRandom->mObject.mNextNextGaussian       = 0.0;
    random.mRandom->mObject.mNextNextGaussianDouble = 0.0;
    random.mRandom->mObject._mt[0]                  = newSeed;

    auto _mtLast = newSeed;
    for (int i = 1; i <= 397; i++) {
        random.mRandom->mObject._mt[i] = i + 0x6C078965 * (_mtLast ^ (_mtLast >> 30));
        _mtLast                        = i + 0x6C078965 * (_mtLast ^ (_mtLast >> 30));
    }
    random.mRandom->mObject._mti       = 624;
    random.mRandom->mObject.mInitedIdx = 398;

    auto able_spacing = mMaxSpacing - mMinSpacing;

    auto random_int_x = (random.nextInt(able_spacing) + random.nextInt(able_spacing)) / 2;
    auto random_int_z = (random.nextInt(able_spacing) + random.nextInt(able_spacing)) / 2;

    ChunkPos target_cpos(able_spacing * (cpos.x / able_spacing) + random_int_x, able_spacing * (cpos.z / able_spacing) + random_int_z);
    if (cpos != target_cpos) return false;
    return true;
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

CustomStructureFeature::CustomStructureFeature(uint seed, uint minSpacing, uint maxSpacing)
: StructureFeature(seed, HashedString("custom:custom_structure_test")),
  mSeed(seed),
  mMinSpacing(minSpacing),
  mMaxSpacing(maxSpacing) {}

} // namespace custom_structure
