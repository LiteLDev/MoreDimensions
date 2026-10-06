#pragma once

#include "mc/world/level/dimension/Dimension.h"                   // base class the new dimension needs
#include "more_dimensions/api/dimension/CustomDimensionManager.h" // uses the DimensionFactoryInfo declaration

// Recommended to use a namespace to avoid clashing with other plugins' class names
namespace nxn_border_terrain {

class NxnBorderTerrainDimension : public Dimension {
    uint chunkLength;

public:
    // Keep this signature: DimensionFactoryInfo carries the basic data used to instantiate the
    // Dimension. `name` is the dimension name, which is what distinguishes dimensions from each other.
    NxnBorderTerrainDimension(std::string const& name, more_dimensions::DimensionFactoryInfo const& info);

    // A method dimensions need; the parameter is whatever extra data you need, e.g. a seed
    static CompoundTag generateNewData(uint chunkLength = 4);

    // The following six have to be overridden.
    // The generator for the dimension's terrain - this is the main thing to change.
    std::unique_ptr<WorldGenerator> createGenerator(br::worldgen::StructureSetRegistry const&) override;

    // Unrelated to this tutorial; just write it as shown.
    void upgradeLevelChunk(ChunkSource& chunkSource, LevelChunk& oldLc, LevelChunk& newLc) override;

    // Unrelated to this tutorial; just write it as shown.
    void fixWallChunk(ChunkSource& cs, LevelChunk& lc) override;

    // Unrelated to this tutorial; just write it as shown.
    bool levelChunkNeedsUpgrade(LevelChunk const& lc) const override;

    // Unrelated to this tutorial; just write it as shown.
    void _upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) override;

    // Unrelated to this tutorial; just write it as shown.
    std::unique_ptr<ChunkSource>
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion ver) override;

    // How coordinates are converted when you transfer into this dimension, e.g. overworld <-> nether
    Vec3 translatePosAcrossDimension(Vec3 const& pos, DimensionType did) const override;

    // Cloud height. Defaults to y128, but the dimension's height range is y-64~320, same as the
    // overworld; override it to put the clouds higher.
    short getCloudHeight() const override;

    // Optional. Changes the visible range while it rains.
    // bool hasPrecipitationFog() const override;
};

} // namespace nxn_border_terrain
