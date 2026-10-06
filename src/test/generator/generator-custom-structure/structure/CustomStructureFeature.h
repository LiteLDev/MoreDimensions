#pragma once

#include "mc/world/level/levelgen/structure/StructureFeature.h"

namespace custom_structure {
class CustomStructureFeature : public StructureFeature {
    uint mSeed;       // The virtuals always receive the default level seed, so store our own
    int  mMaxSpacing; // Max spacing
    int  mMinSpacing; // Min spacing
public:
    bool getNearestGeneratedFeature(
        ::Dimension&                           dimension,
        ::BiomeSource const&                   biomeSource,
        ::BlockPos const&                      origin,
        ::BlockPos&                            pos,
        ::IPreliminarySurfaceProvider const&   preliminarySurfaceLevel,
        bool                                   mustBeInNewChunks,
        ::std::optional<::HashedString> const& biomeTag
    ) override;

    bool isFeatureChunk(
        ::BiomeSource const&,
        ::Random&,
        ::ChunkPos const&,
        uint,
        ::IPreliminarySurfaceProvider const&,
        ::Dimension const&
    ) override;

    ::std::unique_ptr<::StructureStart> createStructureStart(
        ::Dimension&,
        ::BiomeSource const&,
        ::Random&,
        ::ChunkPos const&,
        ::IPreliminarySurfaceProvider const&
    ) override;

    CustomStructureFeature(uint seed, uint minSpacing, uint maxSpacing);
};

} // namespace custom_structure
