#pragma once

#include "more_dimensions/api/dimension/CustomDimensionManager.h"
#include "more_dimensions/core/Macros.h"

#include "mc/world/level/GeneratorType.h"
#include "mc/world/level/dimension/Dimension.h"



class ChunkSource;
class LevelChunk;

namespace more_dimensions {

class SimpleCustomDimension : public Dimension {
    uint          seed;
    GeneratorType generatorType;

public:
    SimpleCustomDimension(std::string const& name, DimensionFactoryInfo const& info);

    static CompoundTag generateNewData(uint seed = 123, GeneratorType generatorType = GeneratorType::Overworld);

    void init(br::worldgen::StructureSetRegistry const&) override;

    std::unique_ptr<WorldGenerator> createGenerator(br::worldgen::StructureSetRegistry const&) override;

    void upgradeLevelChunk(ChunkSource& chunkSource, LevelChunk& oldLc, LevelChunk& newLc) override;

    void fixWallChunk(ChunkSource& cs, LevelChunk& lc) override;

    bool levelChunkNeedsUpgrade(LevelChunk const& lc) const override;

    void _upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) override;

    Vec3 translatePosAcrossDimension(Vec3 const& pos, DimensionType did) const override;

    std::unique_ptr<ChunkSource>
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion ver) override;

    mce::Color getBrightnessDependentFogColor(mce::Color const& color, float brightness) const override;

    short getCloudHeight() const override;
};
} // namespace more_dimensions
