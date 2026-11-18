#pragma once

#include "mc/world/level/dimension/LimboEntitiesVersion.h"

class BlockSource;
class CompoundTag;
class LevelChunk;

/// LeviLamina 26.60 no longer ships mc/world/level/chunk/vanilla_level_chunk_upgrade/VanillaLevelChunkUpgrade.h,
/// but the functions still exist in the game. These wrappers find them by byte signature.
/// If a signature cannot be resolved, an error is logged once and the call becomes a no-op
/// (levelChunkNeedsUpgrade returns false).
namespace more_dimensions::vanilla_upgrade {

void upgradeLevelChunkViaMetaData(LevelChunk& lc, LevelChunk& generatedChunk, BlockSource& region);

void upgradeLevelChunkLegacy(LevelChunk& lc, BlockSource& region);

void fixWallChunk(LevelChunk& lc, BlockSource& region);

bool levelChunkNeedsUpgrade(LevelChunk const& lc);

void upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers, bool isTemplate);

} // namespace more_dimensions::vanilla_upgrade
