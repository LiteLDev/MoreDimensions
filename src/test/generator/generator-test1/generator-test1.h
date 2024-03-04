#pragma once

#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"
#include <vector>

class GeneratorTest1 : public FlatWorldGenerator {
public:
    int chunk_n = 4; // n * n的区块
    std::vector<Block const*> east_side = mPrototypeBlocks;
    std::vector<Block const*> south_side = mPrototypeBlocks;
    std::vector<Block const*> west_side = mPrototypeBlocks;
    std::vector<Block const*> north_side = mPrototypeBlocks;
    std::vector<Block const*> n_e_angle = mPrototypeBlocks;
    std::vector<Block const*> e_s_angle = mPrototypeBlocks;
    std::vector<Block const*> s_w_angle = mPrototypeBlocks;
    std::vector<Block const*> w_n_angle = mPrototypeBlocks;
    std::vector<Block const*> backup_ = mPrototypeBlocks;
    GeneratorTest1(Dimension& dimension, uint seed, Json::Value const& generationOptionsJSON);
    void loadChunk(class LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad);
};
