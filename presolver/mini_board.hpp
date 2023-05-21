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
            assert(valid_counts(brd.placed));
            assert(valid_coronal(2, brd.N));
        }

        uint64_t getBV() const { return m_bv; }
        uint64_t getBH() const { return m_bh; }
        uint64_t getBU() const { return m_bu; }
        uint64_t getBD() const { return m_bd; }

    private:
        /**
         * @brief Validate bit patterns to check if they are a valid coronal placement
         * @param ring_width Width of the coronal ring
         * @param board_size Size of the board
         * @return true if the placement is valid, false otherwise.
         */
        bool valid_coronal(uint8_t ring_width, uint8_t board_size) const {
            // Compute a bitmask where the bits set by the coronal placement are '1'
            const uint32_t hv_ring_mask{bithacks::bits<uint32_t>(0, ring_width - 1) |
                                        bithacks::bits<uint32_t>(board_size - 1 - (ring_width - 1), board_size - 1)};
            // Compute a bitmask where all bits which are a valid queen placement are '1'
            const uint32_t hv_board_mask{bithacks::bits<uint32_t>(0, board_size - 1)};

            // Compute a bitmask where all bits which are a valid queen placement in the diagonals are '1'
            const uint64_t ud_board_mask{bithacks::bits<uint64_t>(0, board_size * 2 - 1)};

            if (m_bv & ~hv_board_mask || m_bh & ~hv_board_mask) {
                // Bits outside the allowed range are set
                return false;
            }

            if (m_bu & ~ud_board_mask || m_bd & ~ud_board_mask) {
                // Bits outside the allowed range are set
                return false;
            }

            if ((m_bv & hv_ring_mask) != hv_ring_mask || (m_bh & hv_ring_mask) != hv_ring_mask) {
                // Bits in the ring are not set
                return false;
            }

            return true;
        }

        /**
         * @brief Validate the number of bits set against the number of queens placed.
         * @param placed Number of queens placed
         * @return true if bit counts are valid, false otherwise.
         */
        bool valid_counts(uint8_t placed) const {
            return bithacks::count_bits_set(m_bu) == placed && bithacks::count_bits_set(m_bd) == placed &&
                   bithacks::count_bits_set(m_bv) == placed && bithacks::count_bits_set(m_bh) == placed;
        }
};
} // namespace queens
