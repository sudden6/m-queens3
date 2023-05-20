#pragma once

#include "board.hpp"
#include "mini_board.hpp"
#include <cstdint>

namespace queens {
static uint64_t countCompletions(uint64_t bv, uint64_t bh, uint64_t bu, uint64_t bd) {
    // Placement Complete?
    if (bh + 1 == 0)
        return 1;

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

} // countCompletions()

static uint64_t countCompletions(queens::mini_board const &brd, uint8_t n) {
    unsigned const N = n;
    return countCompletions(brd.getBV() >> 2, ((((brd.getBH() >> 2) | (~0 << (N - 4))) + 1) << (N - 5)) - 1,
                            brd.getBU() >> 4, (brd.getBD() >> 4) << (N - 5));
}

static uint64_t countCompletions(Board const &brd) {
    return countCompletions(queens::mini_board(brd), brd.N);
}


}; // namespace queens
