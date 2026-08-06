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
    if (ll::getLoaderVersion() < ll::data::Version{0, 8, 3}) {
        getSelf().getLogger().error(
            "The LeviLamina version requires 0.8.3 or higher, now is {}",
            ll::getLoaderVersion().to_string()
        );
        return false;
    }
    getSelf().getLogger().info("More dimension is loaded");
    getSelf().getLogger().info("Version: {}, Developer: LiteLDev", getSelf().getManifest().version->to_string());
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

