#include "NxnBorderTerrainDimension.h"

#include "test/generator/generator-terrain/NxnBorderTerrainGenerator.h"

#include "mc/common/Brightness.h"
#include "mc/common/BrightnessPair.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/DimensionConversionData.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/chunk/vanilla_level_chunk_upgrade/VanillaLevelChunkUpgrade.h"
#include "mc/world/level/dimension/DimensionBrightnessRamp.h"
#include "mc/world/level/dimension/DimensionHeightRange.h"
#include "mc/world/level/dimension/OverworldBrightnessRamp.h"
#include "mc/world/level/dimension/VanillaDimensions.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "mc/world/level/levelgen/structure/VillageFeature.h"
#include "mc/world/level/levelgen/v2/ChunkGeneratorStructureState.h"
#include "mc/world/level/storage/LevelData.h"


namespace nxn_border_terrain {

NxnBorderTerrainDimension::NxnBorderTerrainDimension(
    std::string const&                           name,
    more_dimensions::DimensionFactoryInfo const& info
)
: Dimension(info.level, info.dimId, {-64, 320}, info.scheduler, name) {
    // 这里说明下，在DimensionFactoryInfo里面more-dimensions会提供维度id，请不要使用固定维度id，避免id冲突导致维度注册出现异常
    mDefaultBrightness->sky  = Brightness::MAX();
    mSeaLevel                = -61;
    mHasWeather              = true;
    mDimensionBrightnessRamp = std::make_unique<OverworldBrightnessRamp>();
    mDimensionBrightnessRamp->buildBrightnessRamp();

    // 这个是边长的区块数，是将会传到生成器里面用的
    chunkLength = info.data["ChunkLength"];
}

CompoundTag NxnBorderTerrainDimension::generateNewData(uint chunkLength) {
    CompoundTag result;
    result["ChunkLength"] = chunkLength;
    return result;
}

std::unique_ptr<WorldGenerator> NxnBorderTerrainDimension::createGenerator(br::worldgen::StructureSetRegistry const&) {

    std::unique_ptr<WorldGenerator> worldGenerator;
    auto                            seed      = getLevel().getSeed();
    auto&                           levelData = getLevel().getLevelData();

    // 实例化一个FlatWorldGenerator类
    worldGenerator =
        std::make_unique<NxnBorderTerrainGenerator>(*this, seed, chunkLength, levelData.getFlatWorldGeneratorOptions());
    // 此为必须，一些结构生成相关
    worldGenerator->getStructureFeatureRegistry().mGeneratorState =
        br::worldgen::ChunkGeneratorStructureState::createFlat(seed, worldGenerator->getBiomeSource(), {});

    return std::move(worldGenerator);
}

void NxnBorderTerrainDimension::upgradeLevelChunk(ChunkSource& cs, LevelChunk& lc, LevelChunk& generatedChunk) {
    auto blockSource = BlockSource(getLevel(), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkViaMetaData(lc, generatedChunk, blockSource);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkLegacy(lc, blockSource);
}

void NxnBorderTerrainDimension::fixWallChunk(ChunkSource& cs, LevelChunk& lc) {
    auto blockSource = BlockSource(getLevel(), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::fixWallChunk(lc, blockSource);
}

bool NxnBorderTerrainDimension::levelChunkNeedsUpgrade(LevelChunk const& lc) const {
    return VanillaLevelChunkUpgrade::levelChunkNeedsUpgrade(lc);
}
void NxnBorderTerrainDimension::_upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) {
    auto isTemplate = getLevel().getLevelData().isFromWorldTemplate();
    return VanillaLevelChunkUpgrade::upgradeOldLimboEntity(tag, vers, isTemplate);
}

std::unique_ptr<ChunkSource> NxnBorderTerrainDimension::
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion /*ver*/) {
    return cs;
}

Vec3 NxnBorderTerrainDimension::translatePosAcrossDimension(Vec3 const& fromPos, DimensionType fromId) const {
    Vec3 topos;
    VanillaDimensions::convertPointBetweenDimensions(
        fromPos,
        topos,
        fromId,
        mId,
        getLevel().getDimensionConversionData()
    );
    constexpr auto clampVal = 32000000.0f - 128.0f;

    topos.x = std::clamp(topos.x, -clampVal, clampVal);
    topos.z = std::clamp(topos.z, -clampVal, clampVal);

    return topos;
}

short NxnBorderTerrainDimension::getCloudHeight() const { return 192; }

bool NxnBorderTerrainDimension::hasPrecipitationFog() const { return true; }

} // namespace nxn_border_terrain