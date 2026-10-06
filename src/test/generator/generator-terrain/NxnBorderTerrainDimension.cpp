#include "NxnBorderTerrainDimension.h"

#include "test/generator/generator-terrain/NxnBorderTerrainGenerator.h"

#include "mc/common/Brightness.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/DimensionConversionData.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/source/FixedBiomeSource.h"
#include "mc/world/level/chunk/vanilla_level_chunk_upgrade/VanillaLevelChunkUpgrade.h"
#include "mc/world/level/dimension/DimensionArguments.h"
#include "mc/world/level/dimension/DimensionBrightnessRamp.h"
#include "mc/world/level/dimension/IClientDimensionExtensions.h"
#include "mc/world/level/dimension/OverworldBrightnessRamp.h"
#include "mc/world/level/dimension/VanillaDimensions.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "mc/world/level/levelgen/v2/ChunkGeneratorStructureState.h"
#include "mc/world/level/storage/LevelData.h"

namespace nxn_border_terrain {

NxnBorderTerrainDimension::NxnBorderTerrainDimension(
    std::string const&                           name,
    more_dimensions::DimensionFactoryInfo const& info
)
: Dimension(DimensionArguments(std::move(info.arguments), info.dimId, DimensionHeightRange{-64, 320}, name)) {
    // Note: DimensionFactoryInfo carries the dimension id provided by more-dimensions. Do not
    // hardcode a dimension id, otherwise id collisions will break dimension registration.
    // The height range must match the dimension definition registered in CustomDimensionManager
    // (see the default there), otherwise the client interprets the chunks with a different range.
    // FlatWorldGenerator anchors its layers at mHeightRange.mMin, hence the overworld range.
    mDefaultBrightness->sky  = Brightness::MAX();
    mSeaLevel                = -61;
    mHasWeather              = true;
    mDimensionBrightnessRamp = std::make_unique<OverworldBrightnessRamp>();
    mDimensionBrightnessRamp->buildBrightnessRamp();

    // Number of chunks along one side; this gets passed through to the generator
    chunkLength = info.data["ChunkLength"];
}

CompoundTag NxnBorderTerrainDimension::generateNewData(uint chunkLength) {
    CompoundTag result;
    result["ChunkLength"] = chunkLength;
    return result;
}

std::unique_ptr<WorldGenerator> NxnBorderTerrainDimension::createGenerator(br::worldgen::StructureSetRegistry const&) {

    auto  seed      = mLevel.getSeed();
    auto& levelData = mLevel.getLevelData();

    // Instantiate a FlatWorldGenerator-derived class
    std::unique_ptr<WorldGenerator> worldGenerator =
        std::make_unique<NxnBorderTerrainGenerator>(*this, seed, chunkLength, levelData.mFlatWorldOptions);
    // Required, related to structure generation
    worldGenerator->mStructureFeatureRegistry->mGeneratorState =
        br::worldgen::ChunkGeneratorStructureState::createFlat(seed, worldGenerator->getBiomeSource(), {});

    return std::move(worldGenerator);
}

void NxnBorderTerrainDimension::upgradeLevelChunk(ChunkSource& cs, LevelChunk& lc, LevelChunk& generatedChunk) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false, true);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkViaMetaData(lc, generatedChunk, blockSource);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkLegacy(lc, blockSource);
}

void NxnBorderTerrainDimension::fixWallChunk(ChunkSource& cs, LevelChunk& lc) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false, true);
    VanillaLevelChunkUpgrade::fixWallChunk(lc, blockSource);
}

bool NxnBorderTerrainDimension::levelChunkNeedsUpgrade(LevelChunk const& lc) const {
    return VanillaLevelChunkUpgrade::levelChunkNeedsUpgrade(lc);
}
void NxnBorderTerrainDimension::_upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) {
    auto isTemplate = mLevel.getLevelData().mIsFromLockedTemplate;
    return VanillaLevelChunkUpgrade::upgradeOldLimboEntity(tag, vers, isTemplate);
}

std::unique_ptr<ChunkSource> NxnBorderTerrainDimension::
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion /*ver*/) {
    return cs;
}

Vec3 NxnBorderTerrainDimension::translatePosAcrossDimension(Vec3 const& fromPos, DimensionType fromId) const {
    Vec3 topos;
    VanillaDimensions::convertPointBetweenDimensions(fromPos, topos, fromId, mId, mLevel.getDimensionConversionData());
    constexpr auto clampVal = 32000000.0f - 128.0f;

    topos.x = std::clamp(topos.x, -clampVal, clampVal);
    topos.z = std::clamp(topos.z, -clampVal, clampVal);

    return topos;
}

short NxnBorderTerrainDimension::getCloudHeight() const { return 192; }

// bool NxnBorderTerrainDimension::hasPrecipitationFog() const { return true; }

} // namespace nxn_border_terrain
