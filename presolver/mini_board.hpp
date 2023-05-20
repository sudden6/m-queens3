#pragma once

#include "board.hpp"
#include <cstdint>

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
              m_bh{static_cast<uint32_t>(brd.getBH())} {}

        uint64_t getBV() const { return m_bv; }
        uint64_t getBH() const { return m_bh; }
        uint64_t getBU() const { return m_bu; }
        uint64_t getBD() const { return m_bd; }
};
} // namespace queens
