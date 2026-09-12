#include "MoreDimension.h"

#include "ll/api/Versions.h"
#include "ll/api/mod/RegisterHelper.h"

namespace more_dimensions {

MoreDimension& MoreDimension::getInstance() {
    static MoreDimension instance;
    return instance;
}

bool MoreDimension::load() {
    getSelf().getLogger().info("Loading...");
    getSelf().getLogger().info("More dimension is loaded");
    getSelf().getLogger().info("Version: {}, Developer: LeviMC", getSelf().getManifest().version->to_string());
    return true;
}

bool MoreDimension::enable() {
    getSelf().getLogger().info("Enabling...");
    return true;
}

bool MoreDimension::disable() {
    getSelf().getLogger().info("Disabling...");
    return true;
}

} // namespace more_dimensions

LL_REGISTER_MOD(more_dimensions::MoreDimension, more_dimensions::MoreDimension::getInstance());
