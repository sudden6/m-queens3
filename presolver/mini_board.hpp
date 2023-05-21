#pragma once

#include "board.hpp"
#include <bitset>
#include <cstdint>
#include <numeric>

#include "../bithacks.hpp"

namespace queens {
/**
 * Minimal storage version of board.hpp
 */
class mini_board {
        uint64_t m_bu;
        uint64_t m_bd;
        uint32_t m_bv;
        uint32_t m_bh;

    public:
        mini_board(Board brd)
            : m_bu{brd.getBU()}, m_bd{brd.getBD()}, m_bv{static_cast<uint32_t>(brd.getBV())},
              m_bh{static_cast<uint32_t>(brd.getBH())} {
            assert(valid(brd.placed));
        }

        uint64_t getBV() const { return m_bv; }
        uint64_t getBH() const { return m_bh; }
        uint64_t getBU() const { return m_bu; }
        uint64_t getBD() const { return m_bd; }

    private:
        bool valid(uint8_t placed) {
            return bithacks::count_bits_set(m_bu) == placed && bithacks::count_bits_set(m_bd) == placed &&
                   bithacks::count_bits_set(m_bv) == placed && bithacks::count_bits_set(m_bh) == placed;
        }
};
} // namespace queens
