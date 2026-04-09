#include "CustomDimensionConfig.h"

#include "more_dimensions/MoreDimension.h"
#include "more_dimensions/core/Utils.h"

#include "ll/api/Config.h"
#include "ll/api/service/Bedrock.h"
#include "ll/api/service/ServerInfo.h"
#include "ll/api/utils/Base64Utils.h"
#include "ll/api/utils/ErrorUtils.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "mc/deps/nbt/Tag.h"
#include "mc/server/PropertiesSettings.h"

namespace more_dimensions::CustomDimensionConfig {

// static ll::Logger            logger("CustomDimensionConfig");
auto& logger = MoreDimension::getInstance().getSelf().getLogger();

static std::filesystem::path dimensionConfigPath{u8"./worlds"};

void setDimensionConfigPath() {
    if (!ll::service::getLevel()) {
        throw std::runtime_error("Level nullptr");
    }
#ifdef LL_PLAT_C
    dimensionConfigPath  = ll::getWorldPath().value();
    dimensionConfigPath /= u8"dimension_config.json";
#else
    dimensionConfigPath /= ll::string_utils::str2u8str(ll::service::getPropertiesSettings()->mLevelName);
    dimensionConfigPath /= u8"dimension_config.json";
#endif
}

bool loadConfigFile() {
    if (std::ifstream(dimensionConfigPath).good()) {
        try {
            if (ll::config::loadConfig(
                    getConfig(),
                    dimensionConfigPath,
                    [](Config& config, nlohmann::ordered_json& data) {
                        if (data["version"] < config.version) {
                            for (auto& item : data["dimensionList"]) {
                                auto decompressed = utils::decompress(ll::base64_utils::decode(item["base64Nbt"]));
                                auto nbtTag       = CompoundTag::fromBinaryNbt(decompressed);
                                if (!nbtTag) {
                                    logger.error("Failed to parse NBT from base64Nbt, skipping dimension");
                                    continue;
                                }
                                item["sNbt"] = nbtTag->toSnbt(SnbtFormat::Minimize);
                                item.erase("base64Nbt");
                            }
                        }
                        data.erase("version");
                        auto patch = ll::reflection::serialize<nlohmann::ordered_json>(config);
                        patch.value().merge_patch(data);
                        data = *std::move(patch);
                        return true;
                    }
                )) {
                logger.info("Config file load success!");
                return true;
            }
        } catch (...) {
            logger.error("Config file load fail, will rewrite!");
            ll::error_utils::printCurrentException(logger);
        }
    }
    try {
        if (ll::config::saveConfig(getConfig(), dimensionConfigPath)) {
            logger.warn("Config file rewrite success!");
            return true;
        } else {
            logger.error("Config rewrite failed!");
        }
    } catch (...) {
        logger.error("Config rewrite failed!");
        ll::error_utils::printCurrentException(logger);
    }
    return false;
}

bool saveConfigFile() {
    bool result{};
    try {
        result = ll::config::saveConfig(getConfig(), dimensionConfigPath);
    } catch (...) {
        result = false;
        ll::error_utils::printCurrentException(logger);
    }
    if (!result) {
        logger.error("Config file save fail!");
        return false;
    }
    return true;
}

// void updateConfigVersion() {
//     if (getConfig().version < 4) {
//         logger.info("Config need update");
//         auto config = getConfig();
//         for (auto& item : config.dimensionList) {
//             auto oldStrNbt        = item.second.base64Nbt;
//             item.second.base64Nbt = CompoundTag::fromBinaryNbt(decompress(ll::base64_utils::decode(oldStrNbt)))
//                                         ->toSnbt(SnbtFormat::Minimize);
//         }
//         saveConfigFile();
//     }
// }
} // namespace more_dimensions::CustomDimensionConfig
