#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace more_dimensions {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace more_dimensions
