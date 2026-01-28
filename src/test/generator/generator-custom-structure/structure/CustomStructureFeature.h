#pragma once

#include "mc/world/level/levelgen/structure/StructureFeature.h"

namespace custom_structure {
class CustomStructureFeature : public StructureFeature {
    uint mSeed;       // 虚函数的参数传过来的都是默认的level种子，自定义就存一下
    int  mMaxSpacing; // 最大间距
    int  mMinSpacing; // 最小间距
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
