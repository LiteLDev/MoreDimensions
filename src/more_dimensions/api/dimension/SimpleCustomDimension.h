#pragma once

#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/dimension/NetherBrightnessRamp.h"
#include "mc/world/level/dimension/OverworldBrightnessRamp.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "more_dimensions/api/dimension/CustomDimensionManager.h"

class BaseGameVersion;
class Experiments;
class ChunkSource;
class LevelChunk;

namespace unity_5c986e6b9d6571cc96912b0bfa0329e2 {
MCAPI void addStructureFeatures(
    StructureFeatureRegistry& registry,
    uint                      seed,
    bool                      isLegacy,
    BaseGameVersion const&    baseGameVersion,
    Experiments const&        experiments
);
}
namespace unity_3da1d4c9fa90b4b1becbca96840255a5 {
MCAPI void addStructureFeatures(
    StructureFeatureRegistry& registry,
    uint                      seed,
    BaseGameVersion const&    baseGameVersion,
    Experiments const&        experiments
);
}

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

    bool hasPrecipitationFog() const override;
};
} // namespace more_dimensions
