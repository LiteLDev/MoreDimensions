#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace rename_this {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace rename_this
