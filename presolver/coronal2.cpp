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
// If TRACE is defined the search tree expansion is traced to std::cerr.
// #undef TRACE
// #define TRACE

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string.h>

#include "coronal2.hpp"
#include "symmetry.hpp"

using namespace queens;

Preplacements preplace(unsigned N) {
    std::cout << N << "-Queens Puzzle preplacement generator\n" << std::endl;

    Preplacements res{};

    /**
     * The number of valid pre-placements in two adjacent columns (rows) is
     * 2*(N-2) + (N-2)*(N-3) for the outmost and inner positions in the the
     * first column, respectively. Thus, the total is (N-2)*(N-1).
     */
    struct pres_t {
            char unsigned a;
            char unsigned b;
    };
    std::unique_ptr<pres_t[]> pres(new pres_t[(N - 2) * (N - 1)]);

    { // Compute all valid two-column pre-placements in order:
        // (a0, b0) < (a1, b1) if a0<a1 || (a0==a1 && b0<b1)
        unsigned idx = 0;
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < N; b++) {
                if (abs(static_cast<signed>(a) - static_cast<signed>(b)) <= 1)
                    continue;
                pres[idx].a = a;
                pres[idx].b = b;
                idx++;
            }
        }
        assert(idx == (N - 2) * (N - 1)); // Wrong number of pre-placements
    }
    std::cout << "First side bound: (" << (unsigned)pres[(N / 2) * (N - 3)].a << ", "
              << (unsigned)pres[(N / 2) * (N - 3)].b << ") / (" << (unsigned)pres[(N / 2) * (N - 3) + 1].a << ", "
              << (unsigned)pres[(N / 2) * (N - 3) + 1].b << ')' << std::endl;

    // Generate coronal Placements
    Board board(N);
    for (unsigned w = 0; w <= (N / 2) * (N - 3); w++) {
        unsigned const wa = pres[w].a;
        unsigned const wb = pres[w].b;
#ifdef TRACE
        std::cerr << '(' << wa << ", " << wb << ')' << std::endl;
#else
        std::cout << "\rProgress: " << w << '/' << ((N / 2) * (N - 3)) << std::flush;
#endif

        Board::Placement pwa(board.place(0, wa));
        Board::Placement pwb(board.place(1, wb));
        assert(pwa && pwb); // NO conflicts on first side possible

        for (unsigned n = w; n < (N - 2) * (N - 1) - w; n++) {
            unsigned const na = pres[n].a;
            unsigned const nb = pres[n].b;
#ifdef TRACE
            std::cerr << '(' << wa << ", " << wb << ')' << '(' << na << ", " << nb << ')' << std::endl;
#endif

            Board::Placement pna(board.place(na, N - 1));
            if (!pna)
                continue;
            Board::Placement pnb(board.place(nb, N - 2));
            if (!pnb)
                continue;

            for (unsigned e = w; e < (N - 2) * (N - 1) - w; e++) {
                unsigned const ea = pres[e].a;
                unsigned const eb = pres[e].b;
#ifdef TRACE
                std::cerr << '(' << wa << ", " << wb << ')' << '(' << na << ", " << nb << ')' << '(' << ea << ", " << eb
                          << ')' << std::endl;
#endif

                Board::Placement pea(board.place(N - 1, N - 1 - ea));
                if (!pea)
                    continue;
                Board::Placement peb(board.place(N - 2, N - 1 - eb));
                if (!peb)
                    continue;

                for (unsigned s = w; s < (N - 2) * (N - 1) - w; s++) {
                    unsigned const sa = pres[s].a;
                    unsigned const sb = pres[s].b;
#ifdef TRACE
                    std::cerr << '(' << wa << ", " << wb << ')' << '(' << na << ", " << nb << ')' << '(' << ea << ", "
                              << eb << ')' << '(' << sa << ", " << sb << ')' << std::endl;
#endif

                    Board::Placement psa(board.place(N - 1 - sa, 0));
                    if (!psa)
                        continue;
                    Board::Placement psb(board.place(N - 1 - sb, 1));
                    if (!psb)
                        continue;

                    // We have a successful complete pre-placement with
                    //   w <= n, e, s < (N-2)*(N-1)-w
                    //
                    // Thus, the placement is definitely a canonical minimum unless
                    // one or more of n, e, s are equal to w or (N-2)*(N-1)-1-w.

                    { // Check for minimum if n, e, s = (N-2)*(N-1)-1-w
                        unsigned const ww = (N - 2) * (N - 1) - 1 - w;
                        if (s == ww) {
                            // check if flip about the up diagonal is smaller
                            if (n < (N - 2) * (N - 1) - 1 - e) {
                                // print('S', wa, wb, na, nb, ea, eb, sa, sb);
                                continue;
                            }
                        }
                        if (e == ww) {
                            // check if flip about the vertical center is smaller
                            if (n > (N - 2) * (N - 1) - 1 - n) {
                                // print('E', wa, wb, na, nb, ea, eb, sa, sb);
                                continue;
                            }
                        }
                        if (n == ww) {
                            // check if flip about the down diagonal is smaller
                            if (e > (N - 2) * (N - 1) - 1 - s) {
                                // print('N', wa, wb, na, nb, ea, eb, sa, sb);
                                continue;
                            }
                        }
                    }

                    // Check for minimum if n, e, s = w
                    if (s == w) {
                        // right rotation is smaller unless  w = n = e = s
                        if ((n != w) || (e != w)) {
                            // print('s', wa, wb, na, nb, ea, eb, sa, sb);
                            continue;
                        }
                        res[Symmetry(Symmetry::Direction::ROTATE)].push_back(board);
                        continue;
                    }
                    if (e == w) {
                        // check if 180°-rotation is smaller
                        if (n >= s) {
                            if (n > s) {
                                // print('e', wa, wb, na, nb, ea, eb, sa, sb);
                                continue;
                            }
                            res[Symmetry(Symmetry::Direction::POINT)].push_back(board);
                            continue;
                        }
                    }
                    // n = w is okay

                    // print('o', wa, wb, na, nb, ea, eb, sa, sb);
                    res[Symmetry(Symmetry::Direction::NONE)].push_back(board);
                } // s
            }     // e
        }         // n
    }             // w

    std::cout << std::endl;
    return res;
}
