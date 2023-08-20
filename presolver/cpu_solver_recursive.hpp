#pragma once

#include "board.hpp"
#include "mini_board.hpp"
#include <cstdint>
#include <numeric>

namespace queens {
static uint64_t countCompletions(uint32_t bv, uint64_t bh, uint64_t bu, uint64_t bd) {
    // Placement Complete if all bits (queens) are set
    if (bh == std::numeric_limits<uint64_t>::max()) {
        return 1;
    }

    // -> at least one more queen to place
    while ((bv & 1) != 0) { // Column is covered by pre-placement
        bv >>= 1;
        bu <<= 1;
        bd >>= 1;
    }
    bv >>= 1;

    // Column needs to be placed
    uint64_t cnt = 0;
    for (uint64_t slots = ~(bh | bu | bd); slots != 0;) {
        uint64_t const slot = slots & -slots;
        cnt += countCompletions(bv, bh | slot, (bu | slot) << 1, (bd | slot) >> 1);
        slots ^= slot;
    }

    return cnt;
}

static uint64_t countCompletions(queens::mini_board const &brd, uint8_t n) {
    unsigned const N = n;

    // Compute a bitmask where all bits which are a valid queen placement are '1'
    const uint64_t board_mask{bithacks::bits<uint32_t>(0, N - 1)};

    // TODO: find out why the shift needs to be (N - 7)
    const uint64_t bh_shifted = brd.getBH() << (N - 7);
    const uint64_t board_mask_shifted = board_mask << (N - 7);
    // Need to set all bits that are outside the board range to '1'
    const uint64_t bh_new = bh_shifted | ~board_mask_shifted;
    return countCompletions(brd.getBV() >> 2, bh_new, brd.getBU() >> 4, (brd.getBD() >> 4) << (N - 5));
}

static uint64_t countCompletions(Board const &brd) { return countCompletions(queens::mini_board(brd), brd.N); }

}; // namespace queens
