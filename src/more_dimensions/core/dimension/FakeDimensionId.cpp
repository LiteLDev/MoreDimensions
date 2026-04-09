#include "FakeDimensionId.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/api/dimension/CustomDimensionManager.h"


#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"

#include "mc/deps/core/math/Vec3.h"
#include "mc/deps/core/utility/BinaryStream.h"
#include "mc/entity/components/IPlayerTickPolicy.h"
#include "mc/entity/components/MovementPackets.h"
#include "mc/entity/components/ServerPlayerMovementComponent.h"
#include "mc/legacy/ActorRuntimeID.h"
#include "mc/legacy/ActorUniqueID.h"
#include "mc/network//LoopbackPacketSender.h"
#include "mc/network/MinecraftPacketIds.h"
#include "mc/network/NetworkIdentifierWithSubId.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/AddVolumeEntityPacket.h"
#include "mc/network/packet/ChangeDimensionPacket.h"
#include "mc/network/packet/DebugDrawerPacket.h"
#include "mc/network/packet/LevelChunkPacket.h"
#include "mc/network/packet/PlayerActionPacket.h"
#include "mc/network/packet/PlayerActionType.h"
#include "mc/network/packet/RemoveVolumeEntityPacket.h"
#include "mc/network/packet/ShapeDataPayload.h"
#include "mc/network/packet/SpawnParticleEffectPacket.h"
#include "mc/network/packet/StartGamePacket.h"
#include "mc/network/packet/SubChunkPacket.h"
#include "mc/network/packet/SubChunkRequestPacket.h"
#include "mc/network/packet/UpdateBlockPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/util/VarIntDataOutput.h"
#include "mc/world/level/ChangeDimensionRequest.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/LoadingScreenIdManager.h"
#include "mc/world/level/SpawnSettings.h"
#include "mc/world/level/dimension/VanillaDimensions.h"

MolangScriptArg::MolangScriptArg() = default;

// ChangeDimensionPacket.java
// ClientboundMapItemDataPacket.java
// RemoveVolumeEntityPacket.java
// StartGamePacket.java
// AddVolumeEntityPacket.java
// SpawnParticleEffectPacket.java
// SubChunkPacket.java
// SubChunkRequestPacket.java

namespace more_dimensions {

// static ll::Logger logger("FakeDimensionId");
auto& logger = MoreDimension::getInstance().getSelf().getLogger();

static void sendEmptyChunk(const NetworkIdentifier& netId, int chunkX, int chunkZ, bool forceUpdate) {
    std::array<uchar, 4096> biome{};
    LevelChunkPacket        levelChunkPacket;
    BinaryStream            binaryStream{levelChunkPacket.mSerializedChunk, false};
    VarIntDataOutput        varIntDataOutput(binaryStream);

    varIntDataOutput.writeBytes(&biome, 4096); // write void biome
    for (int i = 1; i <= 8; i++) {
        varIntDataOutput.writeByte(255ui8);
    }
    varIntDataOutput.mStream.writeByte(0, "Byte", nullptr); // write border blocks

    levelChunkPacket.mPos->x         = chunkX;
    levelChunkPacket.mPos->z         = chunkZ;
    levelChunkPacket.mDimensionId    = FakeDimensionId::temporaryDimId;
    levelChunkPacket.mCacheEnabled   = false;
    levelChunkPacket.mSubChunksCount = 0;

    ll::service::getLevel()->getPacketSender()->sendToClient(netId, levelChunkPacket, SubClientId::PrimaryClient);

    if (forceUpdate) {
        UpdateBlockPacket blockPacket;
        blockPacket.mPos         = BlockPos{chunkX << 4, 80, chunkZ << 4};
        blockPacket.mLayer       = 0;
        blockPacket.mUpdateFlags = 1;
        ll::service::getLevel()->getPacketSender()->sendToClient(netId, blockPacket, SubClientId::PrimaryClient);
    }
}

static void sendEmptyChunks(const NetworkIdentifier& netId, const Vec3& position, int radius, bool forceUpdate) {
    int chunkX = static_cast<int>(position.x) >> 4;
    int chunkZ = static_cast<int>(position.z) >> 4;
    for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
            sendEmptyChunk(netId, chunkX + x, chunkZ + z, forceUpdate);
        }
    }
}

static void fakeChangeDimension(
    const NetworkIdentifier& netId,
    ActorRuntimeID           runtimeId,
    DimensionType            fakeDimId,
    const Vec3&              pos,
    std::optional<uint>      screedId
) {
    // ChangeDimensionPacket changeDimensionPacket{fakeDimId, pos, true, {std::nullopt}};
    ChangeDimensionPacket changeDimensionPacket;
    changeDimensionPacket.mDimensionId     = fakeDimId;
    changeDimensionPacket.mPos             = pos;
    changeDimensionPacket.mRespawn         = true;
    changeDimensionPacket.mLoadingScreenId = {screedId};
    ll::service::getLevel()->getPacketSender()->sendToClient(netId, changeDimensionPacket, SubClientId::PrimaryClient);
    PlayerActionPacket playerActionPacket;
    playerActionPacket.mAction    = PlayerActionType::ChangeDimensionAck;
    playerActionPacket.mRuntimeId = runtimeId;
    ll::service::getLevel()->getPacketSender()->sendToClient(netId, playerActionPacket, SubClientId::PrimaryClient);
    sendEmptyChunks(netId, pos, 3, true);
}

namespace CustomDimensionHookList {

namespace sendpackethook {

LL_TYPE_INSTANCE_HOOK(
    LoopbackPacketSendersendToClientHandler1,
    HookPriority::Normal,
    LoopbackPacketSender,
    &LoopbackPacketSender::$sendToClient,
    void,
    UserEntityIdentifierComponent const* userIdentifier,
    Packet const&                        packet
) {
    auto player = ll::service::getServerNetworkHandler()->_getServerPlayer(
        userIdentifier->mNetworkId,
        userIdentifier->mClientSubId
    );
    if (player && player->getDimensionId() >= 3 && packet.getId() != MinecraftPacketIds::ChangeDimension
        && packet.getId() != MinecraftPacketIds::PlayerAction
        && packet.getId() != MinecraftPacketIds::SpawnParticleEffect) {
        FakeDimensionId::changePacketDimension(const_cast<Packet&>(packet));
    }
    if (player && player->getDimensionId() >= 3 && packet.getId() == MinecraftPacketIds::FullChunkData) {
        auto& modifPacket = (LevelChunkPacket&)packet;
        if (modifPacket.mDimensionId->id >= 3) {
            modifPacket.mDimensionId = 0;
        }
        if (modifPacket.mDimensionId->id == VanillaDimensions::Undefined().id) {
            modifPacket.mDimensionId->id = 3;
        }
        if (modifPacket.mClientRequestSubChunkLimit <= 8) {
            modifPacket.mClientRequestSubChunkLimit = 11;
        }
    }
    return origin(userIdentifier, packet);
};

LL_TYPE_INSTANCE_HOOK(
    LoopbackPacketSendersendToClientHandler2,
    HookPriority::Normal,
    LoopbackPacketSender,
    &LoopbackPacketSender::$sendToClient,
    void,
    UserEntityIdentifierComponent const* comp,
    Packet const&                        packet
) {
    auto player = ll::service::getServerNetworkHandler()->_getServerPlayer(comp->mNetworkId, comp->mClientSubId);
    if (!player) return origin(comp, packet);
    auto uuid = player->getUuid();
    if (player && player->getDimensionId() >= 3 && packet.getId() != MinecraftPacketIds::ChangeDimension
        && packet.getId() != MinecraftPacketIds::PlayerAction
        && packet.getId() != MinecraftPacketIds::SpawnParticleEffect) {
        FakeDimensionId::changePacketDimension(const_cast<Packet&>(packet));
    }

    // remove send changeDimensionPacket to client when player die
    if (FakeDimensionId::getInstance().isNeedRemove(uuid) && packet.getId() == MinecraftPacketIds::ChangeDimension) {
        return;
    }
    // remove level event packet, event id is 9801, LevelEvent is SleepingPlayers
    if (packet.getId() == MinecraftPacketIds::LevelEventGeneric && FakeDimensionId::getInstance().isNeedRemove(uuid)) {
        return;
    }
    // remove send changedimension success action packet to client when player die
    if (FakeDimensionId::getInstance().isNeedRemove(uuid) && packet.getId() == MinecraftPacketIds::PlayerAction) {
        auto& actionPacket = (PlayerActionPacket&)packet;
        if (actionPacket.mAction == PlayerActionType::ChangeDimensionAck) {
            return;
        }
    }
    // use fake dimension id when player go to custom dimension
    if (packet.getId() == MinecraftPacketIds::ChangeDimension) {
        auto& modifPacket = (ChangeDimensionPacket&)packet;
        if (modifPacket.mDimensionId->id > 2) {
            modifPacket.mDimensionId->id = FakeDimensionId::fakeDim;
        }
    }
    return origin(comp, packet);
};

LL_TYPE_INSTANCE_HOOK(
    LoopbackPacketSendersendToClientsHandler,
    HookPriority::Normal,
    LoopbackPacketSender,
    &LoopbackPacketSender::$sendToClients,
    void,
    std::vector<NetworkIdentifierWithSubId> const& subIds,
    Packet const&                                  packet
) {
    if (packet.getId() == MinecraftPacketIds::RemoveVolumeEntityPacket
        || packet.getId() == MinecraftPacketIds::AddVolumeEntityPacket) {
        for (auto& subId : subIds) {
            auto player =
                ll::service::getServerNetworkHandler()->_getServerPlayer(subId.id, SubClientId::PrimaryClient);
            if (player && player->getDimensionId() >= 3) {
                FakeDimensionId::changePacketDimension(const_cast<Packet&>(packet));
            }
            packet.sendToClient(subId);
        }
        return;
    }
    return origin(subIds, packet);
};

// StartGamePacket
LL_TYPE_INSTANCE_HOOK(
    StartGamePacketHandler,
    HookPriority::Normal,
    StartGamePacket,
    &StartGamePacket::$ctor,
    void*,
    LevelSettings const&                                                     settings,
    ActorUniqueID                                                            entityId,
    ActorRuntimeID                                                           runtimeId,
    GameType                                                                 entityGameType,
    bool                                                                     enableItemStackNetManager,
    Vec3 const&                                                              pos,
    Vec2 const&                                                              rot,
    std::string const&                                                       levelId,
    std::string const&                                                       levelName,
    ContentIdentity const&                                                   premiumTemplateContentIdentity,
    std::string const&                                                       multiplayerCorrelationId,
    BlockDefinitionGroup const&                                              blockDefinitionGroup,
    bool                                                                     isTrial,
    CompoundTag                                                              playerPropertyData,
    PlayerMovementSettings const&                                            movementSettings,
    std::string const&                                                       serverVersion,
    mce::UUID const&                                                         worldTemplateId,
    std::optional<::ServerConfiguration::ServerConfigurationJoinInfo> const& serverJoinInfo,
    Social::Events::ServerTelemetryData const&                               serverTelemetryData,
    uint64                                                                   levelCurrentTime,
    int                                                                      enchantmentSeed,
    uint64                                                                   blockTypeRegistryChecksum
) {
    if (settings.getSpawnSettings().dimension->id >= 3) {
        SpawnSettings spawnSettings(settings.getSpawnSettings());
        spawnSettings.dimension = FakeDimensionId::fakeDim;
        const_cast<LevelSettings&>(settings).setSpawnSettings(spawnSettings);
    }
    return origin(
        settings,
        entityId,
        runtimeId,
        entityGameType,
        enableItemStackNetManager,
        pos,
        rot,
        levelId,
        levelName,
        premiumTemplateContentIdentity,
        multiplayerCorrelationId,
        blockDefinitionGroup,
        isTrial,
        playerPropertyData,
        movementSettings,
        serverVersion,
        worldTemplateId,
        serverJoinInfo,
        serverTelemetryData,
        levelCurrentTime,
        enchantmentSeed,
        blockTypeRegistryChecksum
    );
}

// ChangeDimensionPacket
// inline function use seedtoClient -> line 200
// LL_TYPE_INSTANCE_HOOK(
//     ChangeDimensionPacketHandler,
//     HookPriority::Normal,
//     ChangeDimensionPacket,
//     &ChangeDimensionPacket::$ctor,
//     void*,
//     DimensionType                  dimId,
//     Vec3                           pos,
//     bool                           respawn,
//     NewType<::std::optional<uint>> loadingScreenId
// ) {
//     if (dimId > 2) {
//         dimId = FakeDimensionId::fakeDim;
//     }
//     return origin(dimId, pos, respawn, loadingScreenId);
// }

// SubChunkPacket and SubChunkRequestPacket
LL_TYPE_INSTANCE_HOOK(
    SubChunkPacketHandler,
    HookPriority::Highest,
    ServerNetworkHandler,
    &ServerNetworkHandler::_buildSubChunkPacketData,
    void,
    NetworkIdentifier const&     networkIdentifier,
    ServerPlayer const*          serverPlayer,
    SubChunkRequestPacket const& subChunkRequestPacket,
    SubChunkPacket&              subChunkPacket,
    uint                         count,
    bool                         isCacheEnabled
) {
    auto inDim = serverPlayer->getDimensionId();
    if (inDim >= 3) {
        const_cast<SubChunkRequestPacket&>(subChunkRequestPacket).mDimensionType = inDim;
        // dimensionType value of subChunkPacket is not read in this function, so it is changed together.
        subChunkPacket.mDimensionType = FakeDimensionId::fakeDim;
    };
    return origin(networkIdentifier, serverPlayer, subChunkRequestPacket, subChunkPacket, count, isCacheEnabled);
}

// SpawnParticleEffectPacket
LL_TYPE_INSTANCE_HOOK(
    SpawnParticleEffectPacketHandler,
    HookPriority::Normal,
    SpawnParticleEffectPacket,
    &SpawnParticleEffectPacket::$ctor,
    void*,
    ::SpawnParticleEffectPacketPayload payload
) {
    if (mVanillaDimensionId >= 3) {
        mVanillaDimensionId = FakeDimensionId::fakeDim.id;
    }
    return origin(std::move(payload));
}

} // namespace sendpackethook

LL_TYPE_INSTANCE_HOOK(
    PlayerdieHandler,
    HookPriority::Normal,
    Player,
    &Player::$die,
    void,
    ActorDamageSource const& actorDamageSource
) {
    if (getDimensionId() >= 3) {
        logger.debug("Remove set true");
        FakeDimensionId::getInstance().setNeedRemove(getUuid(), true);
    }
    return origin(actorDamageSource);
}

namespace receivepackethook {

// when player in overworld and custom dimension will need
LL_TYPE_INSTANCE_HOOK(
    ServerNetworkHandlerPlayerActionPacketHandler,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    NetworkIdentifier const&  netId,
    PlayerActionPacket const& packet
) {
    auto& handler         = ll::memory::dAccess<ServerNetworkHandler>(this, -16);
    auto  player          = handler._getServerPlayer(netId, packet.mSenderSubId);
    auto  uuid            = player->getUuid();
    auto& fakeDimensionId = FakeDimensionId::getInstance();
    if (packet.mAction == PlayerActionType::Respawn) {
        // when player go overworld with die
        if (!fakeDimensionId.isNeedRemove(uuid)) {
            return origin(netId, packet);
        }
        fakeDimensionId.setNeedRemove(uuid, false);
        if (auto moveComp = player->getEntityContext().tryGetComponent<ServerPlayerMovementComponent>()) {
            moveComp->mServerHasMovementAuthority = false;
        }
        fakeDimensionId.onPlayerLeftCustomDimension(uuid, true);
        // flash player bounding box
        // player->getEntityData().markDirty(fmt::underlying(ActorDataIDs::CollisionBox));
    }
    return origin(netId, packet);
};
} // namespace receivepackethook

LL_TYPE_INSTANCE_HOOK(
    LevelrequestPlayerChangeDimensionHandler,
    HookPriority::Normal,
    Level,
    &Level::$requestPlayerChangeDimension,
    void,
    Player&                  player,
    ChangeDimensionRequest&& changeRequest
) {
    auto inId = player.getDimensionId();
    if (changeRequest.mToDimensionId->id == 1 || changeRequest.mToDimensionId->id == 2 || inId.id == 1 || inId.id == 2
        || player.isDead()) {
        return origin(player, std::move(changeRequest));
    };
    // issue #7
    auto loadingScreenIdManager = ll::memory::dAccess<LoadingScreenIdManager*>(&this->mLoadingScreenIdManager, 8);
    auto screedId               = loadingScreenIdManager->mLastLoadingScreenId + 1;
    ++loadingScreenIdManager->mLastLoadingScreenId;
    // screedId.mValue.emplace(screedId.mValue.value() + 1);

    fakeChangeDimension(
        player.getNetworkIdentifier(),
        player.getRuntimeID(),
        more_dimensions::FakeDimensionId::temporaryDimId,
        player.getPosition(),
        screedId
    );
    return origin(player, std::move(changeRequest));
}

using HookReg = ll::memory::HookRegistrar<
    sendpackethook::LoopbackPacketSendersendToClientHandler1,
    sendpackethook::LoopbackPacketSendersendToClientHandler2,
    sendpackethook::LoopbackPacketSendersendToClientsHandler,
    sendpackethook::SubChunkPacketHandler,
    sendpackethook::SpawnParticleEffectPacketHandler,
    sendpackethook::StartGamePacketHandler,
    PlayerdieHandler,
    LevelrequestPlayerChangeDimensionHandler,
    receivepackethook::ServerNetworkHandlerPlayerActionPacketHandler>;

} // namespace CustomDimensionHookList

FakeDimensionId::FakeDimensionId() { CustomDimensionHookList::HookReg::hook(); }

FakeDimensionId::~FakeDimensionId() { CustomDimensionHookList::HookReg::unhook(); }

FakeDimensionId& FakeDimensionId::getInstance() {
    static FakeDimensionId ins{};
    return ins;
}

void FakeDimensionId::changePacketDimension(Packet& packet) {
    switch (auto packId = packet.getId()) {
    case MinecraftPacketIds::RemoveVolumeEntityPacket: {
        auto& tempP          = (RemoveVolumeEntityPacket&)packet;
        tempP.mDimensionType = fakeDim;
        logger.debug("MinecraftPacketIds::RemoveVolumeEntityPacket: dimId change to {}", fakeDim.id);
    }
    case MinecraftPacketIds::AddVolumeEntityPacket: {
        auto& tempP          = (AddVolumeEntityPacket&)packet;
        tempP.mDimensionType = fakeDim;
        logger.debug("MinecraftPacketIds::AddVolumeEntityPacket: dimId change to {}", fakeDim.id);
    }
    case MinecraftPacketIds::DebugDrawerPacket: {
        auto& tempP = (DebugDrawerPacket&)packet;
        for (auto& shape : *tempP.mShapes) {
            shape.mDimensionId = fakeDim;
        }
        logger.debug("MinecraftPacketIds::DebugDrawerPacket: dimId change to {}", fakeDim.id);
    }
    default:
        return;
    }
}

void FakeDimensionId::setNeedRemove(mce::UUID uuid, bool needRemove) {
    std::lock_guard lockGuard{mMapMutex};
    if (mSettingMap.contains(uuid)) {
        mSettingMap.at(uuid).needRemovePacket = needRemove;
    } else {
        mSettingMap.emplace(uuid, CustomDimensionIdSetting{needRemove});
    }
}

bool FakeDimensionId::isNeedRemove(mce::UUID uuid) {
    std::lock_guard lockGuard{mMapMutex};
    if (mSettingMap.contains(uuid)) {
        return mSettingMap.at(uuid).needRemovePacket;
    }
    return false;
}

void FakeDimensionId::onPlayerGoCustomDimension(mce::UUID uuid) {
    std::lock_guard lockGuard{mMapMutex};
    if (!mSettingMap.contains(uuid)) {
        mSettingMap.emplace(uuid, CustomDimensionIdSetting{false});
    }
}

void FakeDimensionId::onPlayerLeftCustomDimension(mce::UUID uuid, bool isRespawn) {
    std::lock_guard lockGuard{mMapMutex};
    if (mSettingMap.contains(uuid)) {
        if (isRespawn) {
            mSettingMap.at(uuid).needRemovePacket = false;
        } else {
            mSettingMap.erase(uuid);
        }
    }
}

} // namespace more_dimensions
