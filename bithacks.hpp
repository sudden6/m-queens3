#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <numeric>

namespace bithacks {

template <typename T> static constexpr auto count_bits_set(T val) {
    static_assert(std::is_integral<T>::value, "Counting bits set only makes sense for integers");
    constexpr auto t_bits = std::numeric_limits<T>::digits;
    return std::bitset<t_bits>(val).count();
}

/**
 * @brief Return a value where the bit N given by the template parameter is set.
 */
template <auto N> static constexpr auto bit() {
    static_assert(N < 64, "Bitwidth not implemented");
    if constexpr (N < 8) {
        return uint8_t{1} << N;
    } else if constexpr (N < 16) {
        return uint16_t{1} << N;
    } else if constexpr (N < 32) {
        return uint32_t{1} << N;
    } else if constexpr (N < 64) {
        return uint64_t{1} << N;
    }
}

/**
 * @brief Return a value where the bit N is set.
 * @param N Bit to set. Zero based index.
 * @return Value with one bit set.
 */
template <typename T> static constexpr T bit(unsigned N) {
    static_assert(std::is_integral<T>::value, "Only integers are sane for bit manipulation");
    // Prevent overflow
    assert(std::numeric_limits<T>::digits > N);
    return T{1} << N;
}

/**
 * @brief Return a value where the bits N to M (inclusive) given by the template parameters are set to '1'.
 */
template <auto N, auto M> static constexpr auto bits() {
    static_assert(N < M, "N must be lower than M");
    // initialize with the highest bit, so the loop index is save from overflow of i. Also helps with
    // type deduction.
    auto res{bit<M>()};
    for (auto i = N; i < M; i++) {
        res |= bit<typeof(res)>(i);
    }

    return res;
}

template <typename T> static constexpr T bits(unsigned N, unsigned M) {
    assert(N < M); // N must be lower than M
    // initialize with the highest bit, so the loop index is save from overflow of i. Will
    // also detect overflows of T.
    T res{bit<T>(M)};
    for (auto i = N; i < M; i++) {
        res |= bit<T>(i);
    }
    return res;
}

} // namespace bithacks
