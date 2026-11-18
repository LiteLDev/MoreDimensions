#include "more_dimensions/api/dimension/VanillaLevelChunkUpgrade.h"

#include "more_dimensions/MoreDimension.h"

#include "ll/api/memory/Signature.h"


namespace more_dimensions::vanilla_upgrade {

using namespace ll::literals::memory_literals;

namespace {

// Signatures of VanillaLevelChunkUpgrade::*, the same on the client (Minecraft.Windows.exe 1.26.60.5)
// and the server (bedrock_server.exe 1.26.60), so no LL_PLAT_C split is needed.
template <class Fn>
Fn resolveOnce(ll::memory::SignatureView sig, char const* name) {
    void* addr = sig.resolve(true);
    auto& logger = MoreDimension::getInstance().getSelf().getLogger();
    if (addr) {
        logger.debug("Resolved VanillaLevelChunkUpgrade::{} at {}", name, addr);
    } else {
        logger.error("Could not resolve VanillaLevelChunkUpgrade::{} by signature; the call is skipped", name);
    }
    return reinterpret_cast<Fn>(addr);
}

} // namespace

void upgradeLevelChunkViaMetaData(LevelChunk& lc, LevelChunk& generatedChunk, BlockSource& region) {
    using Fn       = void (*)(LevelChunk&, LevelChunk&, BlockSource&);
    static auto fn = resolveOnce<Fn>(
        "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 18 01 00 00 48 8D AC 24 ? ? ? ? 0F 29 BD ? ? ? ? 0F 29 75 ? 48 C7 45 ? ? ? ? ? 48 89 CE 0F 57 C0"_sig,
        "_upgradeLevelChunkViaMetaData"
    );
    if (fn) fn(lc, generatedChunk, region);
}

void upgradeLevelChunkLegacy(LevelChunk& lc, BlockSource& region) {
    using Fn       = void (*)(LevelChunk&, BlockSource&);
    static auto fn = resolveOnce<Fn>(
        "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC F8 03 00 00 48 8D AC 24 ? ? ? ? 0F 29 B5 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 48 89 95 ? ? ? ? 80 B9"_sig,
        "_upgradeLevelChunkLegacy"
    );
    if (fn) fn(lc, region);
}

void fixWallChunk(LevelChunk& lc, BlockSource& region) {
    using Fn       = void (*)(LevelChunk&, BlockSource&);
    static auto fn = resolveOnce<Fn>("41 57 41 56 41 55 41 54 56 57 55 53 48 83 EC 68 66 0F 7F 74 24"_sig, "fixWallChunk");
    if (fn) fn(lc, region);
}

bool levelChunkNeedsUpgrade(LevelChunk const& lc) {
    using Fn       = bool (*)(LevelChunk const&);
    static auto fn = resolveOnce<Fn>(
        "55 41 56 56 57 53 48 83 EC 50 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 80 B9 ? ? ? ? ? 75"_sig,
        "levelChunkNeedsUpgrade"
    );
    return fn ? fn(lc) : false;
}

void upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers, bool isTemplate) {
    using Fn       = void (*)(CompoundTag&, ::LimboEntitiesVersion, bool);
    static auto fn = resolveOnce<Fn>(
        "55 41 56 56 57 53 48 81 EC 10 02 00 00 48 8D AC 24 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 44 89 C7 89 D3"_sig,
        "upgradeOldLimboEntity"
    );
    if (fn) fn(tag, vers, isTemplate);
}

} // namespace more_dimensions::vanilla_upgrade
