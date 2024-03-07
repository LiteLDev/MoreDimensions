//
// Created by User on 2024/2/22.
//

#include "FeatureStructureGen.h"

#include "magic_enum.hpp"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/VanillaBiomeNames.h"
#include "mc/world/level/biome/registry/BiomeRegistry.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/block/utils/AirBlockCache.h"
#include "mc/world/level/block/utils/BedrockBlockNames.h"
#include "mc/world/level/block/utils/VanillaBlockTypeIds.h"
#include "mc/world/level/chunk/ChunkViewSource.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/chunk/PostprocessingManager.h"
#include "mc/world/level/levelgen/structure/StructureFeature.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "mc/world/level/levelgen/v1/ChunkLocalNoiseCache.h"

FeatureStructureGen::FeatureStructureGen(Dimension& dimension, uint seed) : WorldGenerator(dimension) {
    mSeed = seed;
    buffer_span_mut<Block const*> buffer;
    mPrototypeBlocks.resize(4096);
    buffer.mBegin      = &*mPrototypeBlocks.begin();
    buffer.mEnd        = &*mPrototypeBlocks.end();
    auto& defaultBlock = BlockTypeRegistry::getDefaultBlockState(BedrockBlockNames::Air, true);
    // 参数说明：后面的48意思维度最低的是y48，会在计算高度时用到
    mPrototype         = BlockVolume(buffer, 16, 16, 16, defaultBlock, 48);
    random.mRandom.mObject._setSeed(seed);

    // 构建BlockVolume，大小是16*16*16，并且里面是填满草方块
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 16; j++) {
            mPrototypeBlocks[16 * i + j] = &BlockTypeRegistry::getDefaultBlockState(VanillaBlockTypeIds::Grass);
        }
    };
    mBiome       = getLevel().getBiomeRegistry().lookupByHash(VanillaBiomeNames::Plains);
    mBiomeSource = std::make_unique<FixedBiomeSource>(*mBiome);
}

bool FeatureStructureGen::postProcess(ChunkViewSource& neighborhood) {
    ChunkPos chunkPos;
    chunkPos.x      = neighborhood.getArea().mBounds.min.x;
    chunkPos.z      = neighborhood.getArea().mBounds.min.z;
    auto levelChunk = neighborhood.getExistingChunk(chunkPos);

    // 必须，需要给区块上锁
    auto lockChunk =
        levelChunk->getDimension().mPostProcessingManager->tryLock(levelChunk->getPosition(), neighborhood);

    if (!lockChunk) {
        return false;
    }
    BlockSource blockSource(getLevel(), neighborhood.getDimension(), neighborhood, false, true, true);
    auto        chunkPosL = levelChunk->getPosition();
    random.mRandom.mObject._setSeed(345);
    auto one = 2 * (random.nextInt() / 2) + 1;
    auto two = 2 * (random.nextInt() / 2) + 1;
    random.mRandom.mObject._setSeed(345 ^ (chunkPosL.x * one + chunkPosL.z * two));
    // 放置结构体
    WorldGenerator::postProcessStructureFeatures(blockSource, random, chunkPosL.x, chunkPosL.z);
    WorldGenerator::postProcessStructures(blockSource, random, chunkPosL.x, chunkPosL.z);
    return true;
}

void FeatureStructureGen::loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad) {
    auto chunkPos = levelchunk.getPosition();

    auto            blockPos = BlockPos(chunkPos, 0);
    DividedPos2d<4> dividedPos2D;
    dividedPos2D.x = (blockPos.x >> 31) - ((blockPos.x >> 31) - blockPos.x) / 4;
    dividedPos2D.z = (blockPos.z >> 31) - ((blockPos.z >> 31) - blockPos.z) / 4;
    WorldGenerator::preProcessStructures(getDimension(), chunkPos, getBiomeSource());
    // 准备要放置的结构
    WorldGenerator::prepareStructureFeatureBlueprints(getDimension(), chunkPos, getBiomeSource(), *this);

    // 后面的112意思是放置BlockVolume时，要向上第112层开始，比如最低y是-64的话，对应的y是-64+112=48
    levelchunk.setBlockVolume(mPrototype, 112);

    levelchunk.recomputeHeightMap(0);
    ChunkLocalNoiseCache chunkLocalNoiseCache(dividedPos2D, 8);
    mBiomeSource->fillBiomes(levelchunk, chunkLocalNoiseCache);
    levelchunk.setSaved();
    levelchunk.changeState(ChunkState::Generating, ChunkState::Generated);
}

std::optional<short> FeatureStructureGen::getPreliminarySurfaceLevel(DividedPos2d<4> worldPos) const {
    auto heightMap = mPrototype.computeHeightMap();
    auto pos_x     = (worldPos.x % 16 + 16) % 16;
    auto pos_z     = (worldPos.z % 16 + 16) % 16;
    short height = heightMap->at(pos_x + 16 * pos_z);
    return height;
}

void FeatureStructureGen::prepareAndComputeHeights(
    BlockVolume&        box,
    ChunkPos const&     chunkPos,
    std::vector<short>& ZXheights,
    bool                factorInBeardsAndShavers,
    int                 skipTopN
) {
    auto heightMap = mPrototype.computeHeightMap();
    ZXheights.assign(heightMap->begin(), heightMap->end());
}

void FeatureStructureGen::prepareHeights(BlockVolume& box, ChunkPos const& chunkPos, bool factorInBeardsAndShavers) {
    box = mPrototype;
};

StructureFeatureType FeatureStructureGen::findStructureFeatureTypeAt(BlockPos const& blockPos) {
    return WorldGenerator::findStructureFeatureTypeAt(blockPos);
};

bool FeatureStructureGen::isStructureFeatureTypeAt(const BlockPos& blockPos, ::StructureFeatureType type) const {
    return WorldGenerator::isStructureFeatureTypeAt(blockPos, type);
}

bool FeatureStructureGen::findNearestStructureFeature(
    ::StructureFeatureType      type,
    BlockPos const&             blockPos,
    BlockPos&                   blockPos1,
    bool                        mustBeInNewChunks,
    std::optional<HashedString> hash
) {
    return WorldGenerator::findNearestStructureFeature(type, blockPos, blockPos1, mustBeInNewChunks, hash);
};

void FeatureStructureGen::garbageCollectBlueprints(buffer_span<ChunkPos> activeChunks) {
    return WorldGenerator::garbageCollectBlueprints(activeChunks);
};

BiomeArea FeatureStructureGen::getBiomeArea(BoundingBox const& area, uint scale) const {
    return mBiomeSource->getBiomeArea(area, scale);
}

WorldGenerator::BlockVolumeDimensions FeatureStructureGen::getBlockVolumeDimensions() const {
    return {mPrototype.mWidth, mPrototype.mDepth, mPrototype.mHeight};
}
