#include "CustomStructureDimension.h"

#include "test/generator/generator-custom-structure/generator/CustomStructureGenerator.h"
#include "test/generator/generator-custom-structure/structure/CustomStructureFeature.h"

#include "mc/common/Brightness.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/DimensionConversionData.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/BiomeIdType.h"
#include "mc/world/level/biome/source/FixedBiomeSource.h"
#include "mc/world/level/chunk/vanilla_level_chunk_upgrade/VanillaLevelChunkUpgrade.h"
#include "mc/world/level/dimension/DimensionArguments.h"
#include "mc/world/level/dimension/IClientDimensionExtensions.h"
#include "mc/world/level/dimension/OverworldBrightnessRamp.h"
#include "mc/world/level/dimension/VanillaDimensions.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "mc/world/level/levelgen/structure/VillageFeature.h"
#include "mc/world/level/levelgen/v2/ChunkGeneratorStructureState.h"
#include "mc/world/level/storage/LevelData.h"


namespace custom_structure_dimension {

CustomStructureDimension::CustomStructureDimension(
    std::string const&                           name,
    more_dimensions::DimensionFactoryInfo const& info
)
: Dimension(DimensionArguments(std::move(info.arguments), info.dimId, {-64, 320}, name)) {
    // Note: DimensionFactoryInfo carries the dimension id provided by more-dimensions. Do not
    // hardcode a dimension id, otherwise id collisions will break dimension registration.
    mDefaultBrightness->sky  = Brightness::MAX();
    mSeaLevel                = -61;
    mHasWeather              = true;
    mDimensionBrightnessRamp = std::make_unique<OverworldBrightnessRamp>();
    mDimensionBrightnessRamp->buildBrightnessRamp();
}

CompoundTag CustomStructureDimension::generateNewData() { return {}; }

std::unique_ptr<WorldGenerator>
CustomStructureDimension::createGenerator(br::worldgen::StructureSetRegistry const& structureSetRegistry) {
    uint  seed      = 2025;
    auto& levelData = mLevel.getLevelData();

    // Instantiate the generator class we wrote
    std::unique_ptr<WorldGenerator> worldGenerator =
        std::make_unique<custom_structure_generator::CustomStructureGenerator>(
            *this,
            seed,
            levelData.mFlatWorldOptions
        );

    // This is effectively how structures get registered in this generator.
    // VillageFeature's 2nd and 3rd parameters are the max/min spacing between villages
    worldGenerator->mStructureFeatureRegistry->mStructureFeatures->emplace_back(
        std::make_unique<VillageFeature>(seed, 34, 8, getBiomeRegistry())
    );
    worldGenerator->mStructureFeatureRegistry->mStructureFeatures->emplace_back(
        std::make_unique<custom_structure::CustomStructureFeature>(seed, 8, 34)
    );
    // Required, related to structure generation
    worldGenerator->mStructureFeatureRegistry->mGeneratorState =
        br::worldgen::ChunkGeneratorStructureState::createFlat(seed, worldGenerator->getBiomeSource(), {});

    return std::move(worldGenerator);
}

void CustomStructureDimension::upgradeLevelChunk(ChunkSource& cs, LevelChunk& lc, LevelChunk& generatedChunk) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false, true);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkViaMetaData(lc, generatedChunk, blockSource);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkLegacy(lc, blockSource);
}

void CustomStructureDimension::fixWallChunk(ChunkSource& cs, LevelChunk& lc) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false, true);
    VanillaLevelChunkUpgrade::fixWallChunk(lc, blockSource);
}

bool CustomStructureDimension::levelChunkNeedsUpgrade(LevelChunk const& lc) const {
    return VanillaLevelChunkUpgrade::levelChunkNeedsUpgrade(lc);
}
void CustomStructureDimension::_upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) {
    auto isTemplate = mLevel.getLevelData().mIsFromLockedTemplate;
    return VanillaLevelChunkUpgrade::upgradeOldLimboEntity(tag, vers, isTemplate);
}

std::unique_ptr<ChunkSource> CustomStructureDimension::
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion /*ver*/) {
    return cs;
}

Vec3 CustomStructureDimension::translatePosAcrossDimension(Vec3 const& fromPos, DimensionType fromId) const {
    Vec3 topos;
    VanillaDimensions::convertPointBetweenDimensions(fromPos, topos, fromId, mId, mLevel.getDimensionConversionData());
    constexpr auto clampVal = 32000000.0f - 128.0f;

    topos.x = std::clamp(topos.x, -clampVal, clampVal);
    topos.z = std::clamp(topos.z, -clampVal, clampVal);

    return topos;
}

short CustomStructureDimension::getCloudHeight() const { return 192; }

// bool CustomStructureDimension::hasPrecipitationFog() const { return true; }

} // namespace custom_structure_dimension
