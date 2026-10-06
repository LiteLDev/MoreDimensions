#include "FlatVillageGenerator.h"

#include "more_dimensions/core/Utils.h"

#include "mc/deps/core/math/Random.h"
#include "mc/util/ThreadOwner.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/ChunkPos.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/Biome.h"
#include "mc/world/level/biome/registry/BiomeRegistry.h"
#include "mc/world/level/biome/source/FixedBiomeSource.h"
#include "mc/world/level/chunk/ChunkState.h"
#include "mc/world/level/chunk/ChunkViewSource.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/chunk/PostprocessingManager.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/v1/ChunkLocalNoiseCache.h"

#include <shared_mutex>

namespace flat_village_generator {

FlatVillageGenerator::FlatVillageGenerator(Dimension& dimension, uint seed, Json::Value const& generationOptionsJSON)
: FlatWorldGenerator(dimension, seed, generationOptionsJSON) {
    // Note: we derive from FlatWorldGenerator and use its internal members later on, so its
    // constructor has to be called
    random.mRandom->mObject.mSeed = seed;
    mSeed                         = seed;

    mBiome       = mLevel->getBiomeRegistry().lookupByName("minecraft:plains");
    mBiomeSource = std::make_unique<FixedBiomeSource>(*mBiome);
}

bool FlatVillageGenerator::structurePostProcessChunk(ChunkViewSource& neighborhood) {
    ChunkPos chunkPos;
    {
        // Since 26.51 GridArea's bounds live inside a SharedLockbox; vanilla takes a shared lock too
        std::shared_lock lock(neighborhood.mArea->mChunkDataBox.mMutex);
        auto const&      bounds = neighborhood.mArea->mChunkDataBox.mContent.mBounds;
        chunkPos.x              = bounds.mMin->x + 1;
        chunkPos.z              = bounds.mMin->z + 1;
    }
    auto levelChunk = neighborhood.getExistingChunk(chunkPos);

    auto seed = mSeed;

    // Required: the chunk has to be locked
    auto lockChunk = levelChunk->mDimension.mPostProcessingManager->tryLock(levelChunk->mPosition, neighborhood);

    if (!lockChunk.has_value()) {
        return false;
    }
    BlockSource blockSource(*mLevel, *neighborhood.mDimension, neighborhood, false, true, true, false);
    auto        chunkPosL         = levelChunk->mPosition;
    random.mRandom->mObject.mSeed = seed;
    auto one                      = 2 * (random.nextInt() / 2) + 1;
    auto two                      = 2 * (random.nextInt() / 2) + 1;
    auto structureSeed            = seed ^ (chunkPosL->x * one + chunkPosL->z * two);
    random.mRandom->mObject.mSeed = structureSeed;
    // Place structures: if this chunk contains one, the structures prepared by loadChunk get placed
    // Since 26.51 postProcessStructureFeatures takes an extra seed parameter; vanilla passes exactly
    // this structure seed
    WorldGenerator::postProcessStructureFeatures(blockSource, random, structureSeed, chunkPosL->x, chunkPosL->z);
    // Handle other standalone structures such as shipwrecks; not required here
    WorldGenerator::postProcessStructures(blockSource, random, chunkPosL->x, chunkPosL->z);

    WorldGenerator::addHardcodedSpawnAreas(*levelChunk);
    return true;
}

void FlatVillageGenerator::loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad) {
    auto chunkPos = levelchunk.mPosition;

    auto            blockPos = BlockPos(chunkPos->x, 0, chunkPos->z);
    DividedPos2d<4> dividedPos2D;
    dividedPos2D.x = (blockPos.x >> 31) - ((blockPos.x >> 31) - blockPos.x) / 4;
    dividedPos2D.z = (blockPos.z >> 31) - ((blockPos.z >> 31) - blockPos.z) / 4;

    // Handle other standalone structures such as shipwrecks; not required here
    WorldGenerator::preProcessStructures(*mDimension, chunkPos, getBiomeSource());
    // Prepare the structures to be placed; if this chunk belongs to one of them, it gets prepared
    WorldGenerator::prepareStructureFeatureBlueprints(*mDimension, chunkPos, getBiomeSource(), *this);

    // Nothing is placed here, only the basic terrain
    levelchunk.setBlockVolume(mPrototype, 0);

    levelchunk.recomputeHeightMap(0);
    ChunkLocalNoiseCache chunkLocalNoiseCache(dividedPos2D, 8);
    mBiomeSource->fillBiomes(levelchunk, &chunkLocalNoiseCache);
    more_dimensions::utils::markChunkAsSaved(levelchunk);
    auto loadState = ChunkState::Generating;
    levelchunk.mLoadState->compare_exchange_strong(loadState, ChunkState::Generated);
}

std::optional<short> FlatVillageGenerator::getPreliminarySurfaceLevel(DividedPos2d<4> worldPos) const {
    // A flat world has the same height everywhere, so just return a fixed value.
    // Must stay consistent with the dimension's sea level (see FlatVillageDimension's ctor).
    return -61;
}

void FlatVillageGenerator::prepareAndComputeHeights(
    BlockVolume&        box,
    ChunkPos const&     chunkPos,
    std::vector<short>& ZXheights,
    bool                factorInBeardsAndShavers,
    int                 skipTopN
) {
    auto heightMap = mPrototype->computeHeightMap();
    ZXheights.assign(heightMap->begin(), heightMap->end());
}

void FlatVillageGenerator::prepareHeights(BlockVolume& box, ChunkPos const& chunkPos, bool factorInBeardsAndShavers) {
    // In other world types the box would be processed here to generate terrain; a flat world does
    // not need that, so just assign it
    box = mPrototype;
};

HashedString FlatVillageGenerator::findStructureFeatureTypeAt(BlockPos const& blockPos) {
    return WorldGenerator::findStructureFeatureTypeAt(blockPos);
};

bool FlatVillageGenerator::isStructureFeatureTypeAt(const BlockPos& blockPos, ::HashedString type) const {
    return WorldGenerator::isStructureFeatureTypeAt(blockPos, type);
}

bool FlatVillageGenerator::findNearestStructureFeature(
    ::HashedString              type,
    BlockPos const&             blockPos,
    BlockPos&                   blockPos1,
    bool                        mustBeInNewChunks,
    std::optional<HashedString> hash
) {
    return WorldGenerator::findNearestStructureFeature(type, blockPos, blockPos1, mustBeInNewChunks, hash);
};

void FlatVillageGenerator::garbageCollectBlueprints(buffer_span<ChunkPos> activeChunks) {
    return WorldGenerator::garbageCollectBlueprints(activeChunks);
};

} // namespace flat_village_generator
