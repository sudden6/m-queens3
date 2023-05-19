#include "cxxopts.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "coronal2.hpp"
#include "symmetry.hpp"

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

    std::cout << "Running with boardsize: " << std::to_string(boardsize) << std::endl;

    auto time_start = std::chrono::high_resolution_clock::now();

    auto preplacements = preplace(boardsize, true);

    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = time_end - time_start;

    std::cout << "Time " << std::to_string(elapsed.count()) << " seconds" << std::endl;

    std::cout << "Preplaced boards:" << std::endl;
    size_t const none = preplacements[queens::Symmetry(queens::Symmetry::Direction::NONE)].size();
    size_t const point = preplacements[queens::Symmetry(queens::Symmetry::Direction::POINT)].size();
    size_t const rotate = preplacements[queens::Symmetry(queens::Symmetry::Direction::ROTATE)].size();

    std::cout << "NONE  : " << std::to_string(none) << std::endl;
    std::cout << "POINT : " << std::to_string(point) << std::endl;
    std::cout << "ROTATE: " << std::to_string(rotate) << std::endl;
    std::cout << "------" << std::endl;
    std::cout << "TOTAL : " << std::to_string(none + point + rotate) << std::endl;

    return 0;
}
