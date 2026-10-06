#include "Utils.h"

#include "mc/world/level/chunk/DirtyTicksCounter.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/chunk/SubChunk.h"
#include "snappy.h"

#include <limits>

namespace more_dimensions::utils {

namespace {

/// @brief Sets a single counter to the "never modified" sentinel value
void resetDirtyTicksCounter(DirtyTicksCounter& counter) {
    counter.totalTime  = std::numeric_limits<int>::min();
    counter.lastChange = 0;
}

} // namespace

std::string compress(std::string_view sv) {
    std::string res;
    snappy::Compress(sv.data(), sv.size(), &res);
    return res;
}

std::string decompress(std::string_view sv) {
    std::string res;
    snappy::Uncompress(sv.data(), sv.size(), &res);
    return res;
}

void markChunkAsSaved(LevelChunk& chunk) {
    for (auto& counter : chunk.mFullChunkDirtyTicksCounters) {
        resetDirtyTicksCounter(*counter);
    }
    for (auto& subChunk : *chunk.mSubChunks) {
        resetDirtyTicksCounter(*subChunk.mDirtyTicksCounter);
    }
    resetDirtyTicksCounter(*chunk.mChunkActorsDirtyTicksCounter);
}

} // namespace more_dimensions::utils
