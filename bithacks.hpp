#pragma once

#include <cstdint>
#include <numeric>
#include <bitset>

namespace bithacks {

template <typename T>
static inline auto count_bits_set(T val) {
    static_assert(std::is_integral<T>::value, "Counting bits set only makes sense for integers");
    constexpr auto t_bits = std::numeric_limits<T>::digits;
    return std::bitset<t_bits>(val).count();
}
}
