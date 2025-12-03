#pragma once

#include <string>
#include <string_view>

namespace more_dimensions::utils {

/// @brief Compress a string using snappy compression
/// @param sv The string view to compress
/// @return The compressed string
std::string compress(std::string_view sv);

/// @brief Decompress a string using snappy decompression
/// @param sv The compressed string view
/// @return The decompressed string
std::string decompress(std::string_view sv);

} // namespace more_dimensions::utils

