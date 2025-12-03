#include "Utils.h"

#include "snappy.h"

namespace more_dimensions::utils {

std::string compress(std::string_view sv) {
    std::string res;
    snappy::Compress(sv.data(), sv.size(), &res);
    return res;
}

std::string decompress(std::string_view sv) {
    std::string res;
    snappy::Uncompress(sv.data(), sv.size(), &res);
    return res;
}

} // namespace more_dimensions::utils

