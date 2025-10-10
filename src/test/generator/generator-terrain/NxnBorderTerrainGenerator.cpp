//
// Created by User on 2024/2/22.
//

#include "NxnBorderTerrainGenerator.h"

#include "test/mc/FixedBiomeSource.h"

#include "mc/world/level/ChunkPos.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/VanillaBlockTypeIds.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/v1/ChunkLocalNoiseCache.h"


namespace nxn_border_terrain {

NxnBorderTerrainGenerator::NxnBorderTerrainGenerator(
    Dimension&         dimension,
    uint               seed,
    uint               chunkLength,
    Json::Value const& generationOptionsJSON
)
: FlatWorldGenerator(dimension, seed, generationOptionsJSON) {
    chunk_n           = chunkLength;
    auto  height      = mPrototype->mHeight;
    auto& roadBlock   = BlockTypeRegistry::get().getDefaultBlockState(VanillaBlockTypeIds::GrassPath());
    auto& borderBlock = BlockTypeRegistry::get().getDefaultBlockState(VanillaBlockTypeIds::SmoothStoneSlab());

    east_side_b.mBlocks  = {east_side.data(), east_side.data() + east_side.size()};
    south_side_b.mBlocks = {south_side.data(), south_side.data() + south_side.size()};
    west_side_b.mBlocks  = {west_side.data(), west_side.data() + west_side.size()};
    north_side_b.mBlocks = {north_side.data(), north_side.data() + north_side.size()};
    n_e_angle_b.mBlocks  = {n_e_angle.data(), n_e_angle.data() + n_e_angle.size()};
    e_s_angle_b.mBlocks  = {e_s_angle.data(), e_s_angle.data() + e_s_angle.size()};
    s_w_angle_b.mBlocks  = {s_w_angle.data(), s_w_angle.data() + s_w_angle.size()};
    w_n_angle_b.mBlocks  = {w_n_angle.data(), w_n_angle.data() + w_n_angle.size()};

    for (int i = 0; i < 16; i++) {
        // 一般开始的是距离第一个方块的差值，然后是会乘
        // i的间隔方块，然后是与另外一条路或者半砖方块的间隔，最后面的是第几层
        // 南边
        e_s_angle[height * 15 + height * 16 * i + 3] = &roadBlock;

        s_w_angle[height * 15 + height * 16 * i + 3] = &roadBlock;

        south_side[height * 15 + height * 16 * i + 3] = &roadBlock;
        south_side[height * 14 + height * 16 * i + 4] = &borderBlock;
        // 东边
        e_s_angle[height * 16 * 15 + height * i + 3] = &roadBlock;

        n_e_angle[height * 16 * 15 + height * i + 3] = &roadBlock;

        east_side[height * 16 * 15 + height * i + 3] = &roadBlock;
        east_side[height * 16 * 14 + height * i + 4] = &borderBlock;
        // 西边
        s_w_angle[height * i + 3] = &roadBlock;

        w_n_angle[height * i + 3] = &roadBlock;

        west_side[height * i + 3]               = &roadBlock;
        west_side[height * 16 + height * i + 4] = &borderBlock;
        // 北边
        n_e_angle[height * 16 * i + 3] = &roadBlock;

        w_n_angle[height * 16 * i + 3] = &roadBlock;

        north_side[height * 16 * i + 3]          = &roadBlock;
        north_side[height + height * 16 * i + 4] = &borderBlock;

        // 半砖避免放到路上，最后一次循环
        if (i < 15) {
            // 南
            e_s_angle[height * 14 + height * 16 * i + 4] = &borderBlock;
            // 东
            e_s_angle[height * 16 * 14 + height * i + 4] = &borderBlock;
            // 西
            s_w_angle[height * 16 + height * i + 4] = &borderBlock;
            // 北
            n_e_angle[height + height * 16 * i + 4] = &borderBlock;
        }
        // 第一次循环
        if (i > 0) {
            // 西
            w_n_angle[height * 16 + height * i + 4] = &borderBlock;
            // 北
            w_n_angle[height + height * 16 * i + 4] = &borderBlock;
            // 南
            s_w_angle[height * 14 + height * 16 * i + 4] = &borderBlock;
            // 东
            n_e_angle[height * 16 * 14 + height * i + 4] = &borderBlock;
        }
    }
}

void NxnBorderTerrainGenerator::loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad) {
    auto chunkPos = levelchunk.mPosition;

    int  n     = chunk_n;
    auto pos_x = (chunkPos->x % n + n) % n;
    auto pos_z = (chunkPos->z % n + n) % n;

    if (pos_x == 0) {
        if (pos_z == 0) {
            levelchunk.setBlockVolume(w_n_angle_b, 0);
        } else if (pos_z == (n - 1)) {
            levelchunk.setBlockVolume(s_w_angle_b, 0);
        } else {
            levelchunk.setBlockVolume(west_side_b, 0);
        }

    } else if (pos_x == (n - 1)) {
        if (pos_z == 0) {
            levelchunk.setBlockVolume(n_e_angle_b, 0);
        } else if (pos_z == (n - 1)) {
            levelchunk.setBlockVolume(e_s_angle_b, 0);
        } else {
            levelchunk.setBlockVolume(east_side_b, 0);
        }
    } else if (pos_z == 0) {
        levelchunk.setBlockVolume(north_side_b, 0);
    } else if (pos_z == (n - 1)) {
        levelchunk.setBlockVolume(south_side_b, 0);
    } else {
        levelchunk.setBlockVolume(mPrototype, 0);
    }

    levelchunk.recomputeHeightMap(false);
    mBiomeSource = std::make_unique<FixedBiomeSource>(*mBiome);
    DividedPos2d<4>      dividedPos2D;
    ChunkLocalNoiseCache chunkLocalNoiseCache(dividedPos2D, 8);
    mBiomeSource->fillBiomes(levelchunk, chunkLocalNoiseCache);
    levelchunk.setSaved();
    auto loadState = ChunkState::Generating;
    levelchunk.mLoadState->compare_exchange_weak(loadState, ChunkState::Generated);
}

} // namespace nxn_border_terrain