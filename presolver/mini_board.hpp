#pragma once

#include "board.hpp"
#include <cstdint>

namespace queens {
/**
 * Minimal storage version of board.hpp
 */
class mini_board {
        // TODO: might be possible to store bv and bh with fewer bits
        uint64_t m_bv;
        uint64_t m_bh;
        uint64_t m_bu;
        uint64_t m_bd;

    public:
        mini_board(Board brd) : m_bv{brd.getBV()}, m_bh{brd.getBH()}, m_bu{brd.getBU()}, m_bd{brd.getBD()} {}

        uint64_t getBV() const { return m_bv; }
        uint64_t getBH() const { return m_bh; }
        uint64_t getBU() const { return m_bu; }
        uint64_t getBD() const { return m_bd; }
};
} // namespace queens
