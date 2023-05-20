#include "cxxopts.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "board.hpp"
#include "coronal2.hpp"
#include "cpu_solver_recursive.hpp"
#include "symmetry.hpp"

// expected results from https://oeis.org/A000170
static constexpr uint64_t results[27] = {1ULL,
                                         0ULL,
                                         0ULL,
                                         2ULL,
                                         10ULL,
                                         4ULL,
                                         40ULL,
                                         92ULL,
                                         352ULL,
                                         724ULL,
                                         2680ULL,
                                         14200ULL,
                                         73712ULL,
                                         365596ULL,
                                         2279184ULL,
                                         14772512ULL,
                                         95815104ULL,
                                         666090624ULL,
                                         4968057848ULL,
                                         39029188884ULL,
                                         314666222712ULL,
                                         2691008701644ULL,
                                         24233937684440ULL,
                                         227514171973736ULL,
                                         2207893435808352ULL,
                                         22317699616364044ULL,
                                         234907967154122528ULL};

int main(int argc, char *argv[]) {
    cxxopts::Options options("m-queens3-presolver", "This program generates work units for the m-queens3 solver");
    // clang-format off
    options.add_options()
        ("N,boardsize", "Size of the board [5..32]", cxxopts::value<uint8_t>())
        ("h,help", "Print usage");
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("boardsize") != 1) {
        std::cout << "Need exactly one board size!" << std::endl;
        return -1;
    }

    const auto boardsize{result["boardsize"].as<uint8_t>()};

    if (boardsize < 5 || boardsize > 32) {
        std::cout << "Boardsize " << std::to_string(boardsize) << " is out of limits 5..32" << std::endl;
        return -1;
    }

    // Compute preplacements
    std::cout << "Running with boardsize: " << std::to_string(boardsize) << std::endl;
    std::cout.precision(3);

    auto time_start = std::chrono::high_resolution_clock::now();

    auto preplacements = preplace(boardsize);

    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = time_end - time_start;

    { // Preplacement stats
        std::cout << "Preplaced boards:" << std::endl;
        size_t const none = preplacements[queens::Symmetry(queens::Symmetry::Direction::NONE)].size();
        size_t const point = preplacements[queens::Symmetry(queens::Symmetry::Direction::POINT)].size();
        size_t const rotate = preplacements[queens::Symmetry(queens::Symmetry::Direction::ROTATE)].size();
        size_t const total = none + point + rotate;

        // Board object does dynamic memory allocation
        size_t const board_obj_size = sizeof(queens::Board) + boardsize * sizeof(int8_t);

        std::cout << "NONE  : " << std::to_string(none) << std::endl;
        std::cout << "POINT : " << std::to_string(point) << std::endl;
        std::cout << "ROTATE: " << std::to_string(rotate) << std::endl;
        std::cout << "------" << std::endl;
        std::cout << "TOTAL : " << std::to_string(total) << std::endl;
        std::cout << "Memory: " << std::to_string(total*board_obj_size) << std::endl;
        std::cout << "Time  : " << elapsed.count() << " seconds" << std::endl;
    }

    std::cout << std::endl;

    // Solve preplacements

    time_start = std::chrono::high_resolution_clock::now();

    std::array<uint64_t, queens::ALL_SYMMETRIES.size()> counts{};

    for (queens::Symmetry const &sym : queens::ALL_SYMMETRIES) {
        uint64_t l_counts = 0;
        #pragma omp parallel for reduction(+ : l_counts) schedule(dynamic)
        for (queens::Board const &brd : preplacements[sym]) {
            l_counts += queens::countCompletions(brd);
        }
        counts[sym] = l_counts;
    }

    time_end = std::chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;

    { // Solver stats
        uint64_t const none = counts[queens::Symmetry(queens::Symmetry::Direction::NONE)] *
                              queens::Symmetry(queens::Symmetry::Direction::NONE).weight();
        uint64_t const point = counts[queens::Symmetry(queens::Symmetry::Direction::POINT)] *
                               queens::Symmetry(queens::Symmetry::Direction::POINT).weight();
        uint64_t const rotate = counts[queens::Symmetry(queens::Symmetry::Direction::ROTATE)] *
                                queens::Symmetry(queens::Symmetry::Direction::ROTATE).weight();
        uint64_t const total = none + point + rotate;

        std::cout << "Solutions:" << std::endl;
        std::cout << "NONE  : " << std::to_string(none) << std::endl;
        std::cout << "POINT : " << std::to_string(point) << std::endl;
        std::cout << "ROTATE: " << std::to_string(rotate) << std::endl;
        std::cout << "------" << std::endl;
        std::cout << "TOTAL : " << std::to_string(total) << std::endl;
        std::cout << "Time  : " << elapsed.count() << " seconds, ";
        std::cout << "Solutions/s " << total/elapsed.count() << std::endl;

        std::cout << (results[boardsize - 1] == total ? "PASS" : "FAIL") << std::endl;
    }

    return 0;
}
