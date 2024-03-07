#pragma once

#include "mc/util/Random.h"
#include "mc/world/level/biome/source/BiomeArea.h"
#include "mc/world/level/biome/source/FixedBiomeSource.h"
#include "mc/world/level/block/BlockVolume.h"
#include "mc/world/level/levelgen/WorldGenerator.h"

#include <vector>

class ChunkViewSource;
class LevelChunk;
class ChunkPos;
class BlockVolumeTarget;
class BlockSource;
class Block;

/*
 * 这是一个关于功能性结构（FeatureStructure）生成相关的生成器示例
 * */

class FeatureStructureGen : public WorldGenerator {
public:
    std::vector<Block const*>         mPrototypeBlocks; // this+0x190
    BlockVolume                       mPrototype;       // this+0x1A8
    Biome const*                      mBiome;           // this+0x1D0
    std::unique_ptr<FixedBiomeSource> mBiomeSource;     // this+0x1D8
    uint                              mSeed;
    Random                            random;
    FeatureStructureGen(Dimension& dimension, uint seed);

    // 这里是处理中结构放置相关的，包括地物，结构，一些地形
    bool postProcess(ChunkViewSource& neighborhood);

    // 这里是初始处理新的单区块的方块生成相关的，比如一些大量的方块（石头，泥土）
    void loadChunk(LevelChunk& levelchunk, bool forceImmediateReplacementDataLoad);

    StructureFeatureType findStructureFeatureTypeAt(BlockPos const&);

    bool isStructureFeatureTypeAt(BlockPos const&, ::StructureFeatureType) const;

    // 这里是获取某个坐标的最高方块
    std::optional<short> getPreliminarySurfaceLevel(class DividedPos2d<4> worldPos) const;

    // 如意，以一个坐标，在一定范围内查找某个类型的结构
    bool
    findNearestStructureFeature(::StructureFeatureType, BlockPos const&, BlockPos&, bool, std::optional<HashedString>);

    void garbageCollectBlueprints(buffer_span<ChunkPos>);

    // 处理地形
    void prepareHeights(BlockVolume& box, ChunkPos const& chunkPos, bool factorInBeardsAndShavers);

    // 与prepareHeights一样，不过与之不同的是，还会计算单区块内的高度
    void prepareAndComputeHeights(
        BlockVolume&        box,
        ChunkPos const&     chunkPos,
        std::vector<short>& ZXheights,
        bool                factorInBeardsAndShavers,
        int                 skipTopN
    );

    BiomeArea getBiomeArea(BoundingBox const& area, uint scale) const;

    BiomeSource const& getBiomeSource() const { return *mBiomeSource; }

    WorldGenerator::BlockVolumeDimensions getBlockVolumeDimensions() const;

    BlockPos findSpawnPosition() const { return {0, 16, 0}; };

    void decorateWorldGenLoadChunk(
        Biome&             biome,
        LevelChunk&        lc,
        BlockVolumeTarget& target,
        Random&            random,
        ChunkPos const&    pos
    ) const {};

    void decorateWorldGenPostProcess(Biome& biome, LevelChunk& lc, BlockSource& source, Random& random) const {};
};
