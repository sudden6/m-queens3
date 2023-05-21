/*****************************************************************************
 * This file is part of the Queens@TUD solver suite
 * for enumerating and counting the solutions of an N-Queens Puzzle.
 *
 * Copyright (C) 2008-2015
 *      Thomas B. Preusser <thomas.preusser@utexas.edu>
 *****************************************************************************
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#pragma once

#include <cstdint>
#include <iostream>

#include <assert.h>

namespace queens {
class Board {
    public:
        uint8_t const N;
        uint8_t placed;

    private:
        int8_t *const board;

        uint64_t bv;
        uint64_t bh;
        uint64_t bu;
        uint64_t bd;

    public:
        Board(unsigned const dim) : N(dim), placed(0), board(new int8_t[N]), bv(0), bh(0), bu(0), bd(0) {
            for (uint8_t i = 0; i < N; i++) {
                board[i] = -1;
            }
        }
        ~Board() { delete[] board; }
        Board(Board const &other)
            : N(other.N), placed(other.placed), board(new int8_t[N]), bv(other.bv), bh(other.bh), bu(other.bu),
              bd(other.bd) {
            for (uint8_t i = 0; i < N; i++) {
                board[i] = other.board[i];
            }
        }

    private:
        class Cell {
                int8_t &col;
                unsigned const y;

            public:
                Cell(int8_t &_col, unsigned const _y) : col(_col), y(_y) {}
                ~Cell() {}

            public:
                operator bool() const { return col == (int8_t)y; }
                Cell &operator=(bool const v) {
                    if (v) {
                        assert(col == -1);
                        col = (int8_t)y;
                    } else {
                        assert(col == (int8_t)y);
                        col = -1;
                    }
                    return *this;
                }
        }; // class Cell

    public:
        bool operator()(unsigned x, unsigned y) const { return board[x] == (int8_t)y; }

    private:
        Cell operator()(unsigned x, unsigned y) { return Cell(board[x], y); }

    public:
        class Placement {
                Board &parent;

            public:
                unsigned const x;
                unsigned const y;

            private:
                bool valid;
                bool owner;

            private:
                friend class Board;
                Placement(Board &_parent, unsigned const _x, unsigned const _y) : parent(_parent), x(_x), y(_y) {
                    if (parent(x, y)) {
                        // Duplicate Placement
                        valid = true;
                        owner = false;
                        return;
                    }

                    // Check Validity of new Placement
                    uint64_t const bv = UINT64_C(1) << x;
                    uint64_t const bh = UINT64_C(1) << y;
                    uint64_t const bu = UINT64_C(1) << (parent.N - 1 - x + y);
                    uint64_t const bd = UINT64_C(1) << (x + y);
                    if ((parent.bv & bv) || (parent.bh & bh) || (parent.bu & bu) || (parent.bd & bd)) {
                        valid = false;
                        owner = false;
                        return;
                    }
                    parent(x, y) = true;
                    parent.bv |= bv;
                    parent.bh |= bh;
                    parent.bu |= bu;
                    parent.bd |= bd;
                    parent.placed += 1;
                    valid = true;
                    owner = true;
                }

            public:
                ~Placement() {
                    if (owner) {
                        parent.bv ^= UINT64_C(1) << x;
                        parent.bh ^= UINT64_C(1) << y;
                        parent.bu ^= UINT64_C(1) << (parent.N - 1 - x + y);
                        parent.bd ^= UINT64_C(1) << (x + y);
                        parent.placed -= 1;
                        parent(x, y) = false;
                    }
                }

            public:
                operator bool() { return valid; }

        }; // class Placement

        Placement place(unsigned x, unsigned y) { return Placement(*this, x, y); }

        uint64_t getBV() const { return bv; }
        uint64_t getBH() const { return bh; }
        uint64_t getBU() const { return bu; }
        uint64_t getBD() const { return bd; }
        uint8_t getPlaced() const { return placed; }

        unsigned coronal(int8_t *buf, uint8_t rings) const {
            if (rings > N)
                rings = N;

            for (uint8_t x = 0; x < rings; x++) {
                *buf++ = board[x];
            }
            for (uint8_t y = N; y-- > N - rings;) {
                *buf = -1;
                for (uint8_t x = 0; x < N; x++) {
                    if (operator()(x, y)) {
                        *buf = x;
                        break;
                    }
                }
                buf++;
            }
            for (uint8_t x = N; x-- > N - rings;) {
                *buf++ = N - 1 - board[x];
            }
            for (uint8_t y = 0; y < rings; y++) {
                *buf = -1;
                for (uint8_t x = 0; x < N; x++) {
                    if (operator()(x, y)) {
                        *buf = N - 1 - x;
                        break;
                    }
                }
                buf++;
            }
            return N;

        } // coronal

}; // class Board

inline std::ostream &operator<<(std::ostream &out, Board const &brd) {
    uint8_t const N = brd.N;
    for (uint8_t y = N; y-- > 0;) {
        for (uint8_t x = 0; x < N; x++) {
            out << (brd(x, y) ? 'Q' : '.');
        }
        out << std::endl;
    }
    return out;
}

inline std::ostream &operator<<(std::ostream &out, Board::Placement const &p) {
    out << '(' << p.x << ',' << p.y << ')';
    return out;
}

} // namespace queens
