//
// Created by User on 2024/2/22.
//

#include "generator-terrain1.h"

#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/block/utils/BedrockBlockNames.h"
#include "mc/world/level/block/utils/VanillaBlockTypeIds.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/levelgen/v1/ChunkLocalNoiseCache.h"

GeneratorTerrain1::GeneratorTerrain1(Dimension& dimension, uint seed, Json::Value const& generationOptionsJSON):
FlatWorldGenerator(dimension, seed, generationOptionsJSON){
    auto height = mPrototype.mHeight;
    for (int i = 0; i < 16; i++) {
        // 一般开始的是距离第一个方块的差值，然后是会乘 i的间隔方块，然后是与另外一条路或者半砖方块的间隔，最后面的是第几层
        // 南边
        e_s_angle[height*15+height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        s_w_angle[height*15+height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        south_side[height*15+height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);
        south_side[height*14+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
        // 东边
        e_s_angle[height*16*15+height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        n_e_angle[height*16*15+height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        east_side[height*16*15+height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);
        east_side[height*16*14+height*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
        // 西边
        s_w_angle[height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        w_n_angle[height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        west_side[height*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);
        west_side[height*16+height*i+4] = &BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
        // 北边
        n_e_angle[height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        w_n_angle[height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);

        north_side[height*16*i+3]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::GrassPath);
        north_side[height+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);

        // 半砖避免放到路上，最后一次循环
        if (i < 15) {
            // 南
            e_s_angle[height*14+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 东
            e_s_angle[height*16*14+height*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 西
            s_w_angle[height*16+height*i+4] = &BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 北
            n_e_angle[height+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
        }
        // 第一次循环
        if ( i > 0) {
            // 西
            w_n_angle[height*16+height*i+4] = &BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 北
            w_n_angle[height+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 南
            s_w_angle[height*14+height*16*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
            // 东
            n_e_angle[height*16*14+height*i+4]=&BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::StoneSlab);
        }
    }
}

void GeneratorTerrain1::loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad) {
    auto chunkPos = levelchunk.getPosition();

    int n = chunk_n;
    auto pos_x = (chunkPos.x % n + n) % n;
    auto pos_z = (chunkPos.z % n + n) % n;
    auto height = mPrototype.mHeight;
    auto minHeight = mDimension->getMinHeight();
    auto& defaultBlock = BlockTypeRegistry::getDefaultBlockState(BedrockBlockNames::Air,true);
    buffer_span_mut<Block const*> buffer;
    if(pos_x == 0) {
        if (pos_z == 0) {
            buffer.mBegin = &*w_n_angle.begin();
            buffer.mEnd = &*w_n_angle.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        } else if (pos_z == (n-1)){
            buffer.mBegin = &*s_w_angle.begin();
            buffer.mEnd = &*s_w_angle.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        } else {
            buffer.mBegin = &*west_side.begin();
            buffer.mEnd = &*west_side.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        }

    } else if ( pos_x == (n-1)) {
        if (pos_z == 0) {
            buffer.mBegin = &*n_e_angle.begin();
            buffer.mEnd = &*n_e_angle.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        } else if (pos_z == (n-1)){
            buffer.mBegin = &*e_s_angle.begin();
            buffer.mEnd = &*e_s_angle.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        } else {
            buffer.mBegin = &*east_side.begin();
            buffer.mEnd = &*east_side.end();
            levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
        }
    } else if ( pos_z == 0){
        buffer.mBegin = &*north_side.begin();
        buffer.mEnd = &*north_side.end();
        levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
    } else if (pos_z == (n-1)) {
        buffer.mBegin = &*south_side.begin();
        buffer.mEnd = &*south_side.end();
        levelchunk.setBlockVolume(BlockVolume(buffer, 16, height, 16, defaultBlock, minHeight), 0);
    } else {
        levelchunk.setBlockVolume(mPrototype, 0);
    }


    levelchunk.recomputeHeightMap(0);
    mBiomeSource = std::make_unique<FixedBiomeSource>(*mBiome);
    ChunkLocalNoiseCache chunkLocalNoiseCache;
    mBiomeSource->fillBiomes(levelchunk, chunkLocalNoiseCache);
    levelchunk.setSaved();
    levelchunk.changeState(ChunkState::Generating, ChunkState::Generated);
}
