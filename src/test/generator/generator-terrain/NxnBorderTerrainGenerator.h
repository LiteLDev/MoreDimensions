#pragma once

#include "mc/world/level/block/BlockVolume.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"

#include <vector>

namespace nxn_border_terrain {

class NxnBorderTerrainGenerator : public FlatWorldGenerator {
public:
    uint                      chunk_n      = 4; // n * n的区块
    std::vector<Block const*> east_side    = mPrototypeBlocks;
    std::vector<Block const*> south_side   = mPrototypeBlocks;
    std::vector<Block const*> west_side    = mPrototypeBlocks;
    std::vector<Block const*> north_side   = mPrototypeBlocks;
    std::vector<Block const*> n_e_angle    = mPrototypeBlocks;
    std::vector<Block const*> e_s_angle    = mPrototypeBlocks;
    std::vector<Block const*> s_w_angle    = mPrototypeBlocks;
    std::vector<Block const*> w_n_angle    = mPrototypeBlocks;
    BlockVolume               east_side_b  = mPrototype;
    BlockVolume               south_side_b = mPrototype;
    BlockVolume               west_side_b  = mPrototype;
    BlockVolume               north_side_b = mPrototype;
    BlockVolume               n_e_angle_b  = mPrototype;
    BlockVolume               e_s_angle_b  = mPrototype;
    BlockVolume               s_w_angle_b  = mPrototype;
    BlockVolume               w_n_angle_b  = mPrototype;
    NxnBorderTerrainGenerator(
        Dimension&         dimension,
        uint               seed,
        uint               chunkLength,
        Json::Value const& generationOptionsJSON
    );
    void loadChunk(class LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad);
};
} // namespace nxn_border_terrain