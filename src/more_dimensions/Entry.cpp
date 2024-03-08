#include "Entry.h"

#include "fmt/format.h"
#include "functional"
#include "ll/api/Config.h"
#include "ll/api/io/FileUtils.h"
#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/PluginManagerRegistry.h"
#include "ll/api/service/ServerInfo.h"
#include "more_dimensions/core/Macros.h"

#include <memory>
#include <stdexcept>

namespace more_dimensions {

namespace {

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

std::unique_ptr<std::reference_wrapper<ll::plugin::NativePlugin>> selfPluginInstance;

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

} // namespace

auto disable(ll::plugin::NativePlugin& self) -> bool;
auto enable(ll::plugin::NativePlugin& self) -> bool;
auto load(ll::plugin::NativePlugin& self) -> bool;
auto unload(ll::plugin::NativePlugin& self) -> bool;

extern "C" {
_declspec(dllexport) auto ll_plugin_disable(ll::plugin::NativePlugin& self) -> bool { return disable(self); }
_declspec(dllexport) auto ll_plugin_enable(ll::plugin::NativePlugin& self) -> bool { return enable(self); }
_declspec(dllexport) auto ll_plugin_load(ll::plugin::NativePlugin& self) -> bool { return load(self); }
_declspec(dllexport) auto ll_plugin_unload(ll::plugin::NativePlugin& self) -> bool { return unload(self); }
}

auto disable(ll::plugin::NativePlugin& /*self*/) -> bool {
    auto& logger = getSelfPluginInstance().getLogger();

    logger.info("disabling...");

    // Your code here.

    logger.info("disabled");

    return true;
}

auto enable(ll::plugin::NativePlugin& /*self*/) -> bool {
    auto& logger = getSelfPluginInstance().getLogger();

    logger.info("enabling...");

    logger.info("enabled");

    return true;
}

auto getSelfPluginInstance() -> ll::plugin::NativePlugin& {
    if (!selfPluginInstance) {
        throw std::runtime_error("selfPluginInstance is null");
    }

    return *selfPluginInstance;
}

auto load(ll::plugin::NativePlugin& self) -> bool {
    auto& logger = self.getLogger();

    logger.info("loading...");
    if (ll::getLoaderVersion() < ll::data::Version{0, 8, 3}) {
        logger.error("The LeviLamina version requires 0.8.3 or higher, now is {}", ll::getLoaderVersion().to_string());
        return false;
    }
    selfPluginInstance = std::make_unique<std::reference_wrapper<ll::plugin::NativePlugin>>(self);
    logger.info("More dimension is loaded");
    logger.info("Version: {}, Developer: LiteLDev", VERSION);

    return true;
}

auto unload(ll::plugin::NativePlugin& self) -> bool {
    auto& logger = self.getLogger();

    logger.info("unloading...");

    selfPluginInstance.reset();

    // Your code here.

    logger.info("unloaded");

    return true;
}

} // namespace more_dimensions
