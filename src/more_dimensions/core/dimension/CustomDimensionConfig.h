#pragma once

#include <string>
#include <unordered_map>


namespace more_dimensions::CustomDimensionConfig {
struct Config {
    struct Info {
        int         dimId{};
        std::string sNbt;
    };
    int                                   version = 4;
    std::unordered_map<std::string, Info> dimensionList{};
};

inline Config& getConfig() {
    static Config instance;
    return instance;
}

void setDimensionConfigPath();
bool loadConfigFile();
bool saveConfigFile();
}; // namespace more_dimensions::CustomDimensionConfig
