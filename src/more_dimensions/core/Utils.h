#pragma once

#include <string>
#include <string_view>

class LevelChunk;

namespace more_dimensions::utils {

/// @brief Compress a string using snappy compression
/// @param sv The string view to compress
/// @return The compressed string
std::string compress(std::string_view sv);

/// @brief Decompress a string using snappy decompression
/// @param sv The compressed string view
/// @return The decompressed string
std::string decompress(std::string_view sv);

/// @brief Resets every dirty-ticks counter of the chunk to the "never modified" sentinel value
///
/// Bedrock removed LevelChunk::setSaved() in 26.32, along with the whole save-state API
/// (setUnsaved, clearDirtyTickCounters, ...). All it did was set the six
/// mFullChunkDirtyTicksCounters, every SubChunk::mDirtyTicksCounter and
/// mChunkActorsDirtyTicksCounter to totalTime = INT_MIN, lastChange = 0. 26.51's vanilla
/// FlatWorldGenerator::loadChunk has that logic inlined. Custom generators that override
/// loadChunk can call this to reproduce the old behaviour, so that freshly generated terrain
/// is not immediately considered dirty by LevelChunk::nonActorDataNeedsSaving().
/// @param chunk The chunk to mark
/// @note Only resets the dirty timers; it does not persist anything
void markChunkAsSaved(LevelChunk& chunk);

} // namespace more_dimensions::utils
