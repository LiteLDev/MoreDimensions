#pragma once

#include "mc/deps/core/string/HashedString.h"
#include "mc/deps/core/utility/buffer_span.h"
#include "mc/util/Random.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/DividedPos2d.h"
#include "mc/world/level/block/BlockVolume.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"


#include <vector>

class ChunkViewSource;
class LevelChunk;
class ChunkPos;

// Still recommended to use a namespace to avoid collisions
namespace custom_structure_generator {

// Deriving from vanilla's flat world generator makes this a lot easier
class CustomStructureGenerator : public FlatWorldGenerator {
public:
    // 26.51 no longer exports Random's default constructor; use createSeedable() to build an
    // instance that owns its own seed
    Random random = Random::createSeedable(); // BDS class related to random number generation
    uint   mSeed;

    // generationOptionsJSON is unused here, but FlatWorldGenerator's instantiation requires it
    CustomStructureGenerator(Dimension& dimension, uint seed, Json::Value const& generationOptionsJSON);

    // Handles structure placement, including features, structures and terrain
    bool postProcess(ChunkViewSource& neighborhood);

    // Handles the initial block generation of a new chunk, e.g. bulk blocks (stone, dirt)
    void loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad) override;

    // Tells which structure range a given position falls into
    HashedString findStructureFeatureTypeAt(BlockPos const&) override;

    // Tells whether a given position is inside a structure range
    bool isStructureFeatureTypeAt(BlockPos const&, ::HashedString) const override;

    // Returns the highest block at a given position
    std::optional<short> getPreliminarySurfaceLevel(DividedPos2d<4> worldPos) const override;

    // Finds the nearest structure of the given type within a radius around a position
    bool
    findNearestStructureFeature(::HashedString, BlockPos const&, BlockPos&, bool, std::optional<HashedString>) override;

    // Not important, just implement it as-is
    void garbageCollectBlueprints(buffer_span<ChunkPos>) override;

    // Generates terrain
    void prepareHeights(BlockVolume& box, ChunkPos const& chunkPos, bool factorInBeardsAndShavers);

    // Same as prepareHeights, except that it also computes the heights inside a single chunk
    void prepareAndComputeHeights(
        BlockVolume&        box,
        ChunkPos const&     chunkPos,
        std::vector<short>& ZXheights,
        bool                factorInBeardsAndShavers,
        int                 skipTopN
    );

    // Optional, can be omitted
    BlockPos findSpawnPosition() const override { return {0, 16, 0}; }
};

} // namespace custom_structure_generator
