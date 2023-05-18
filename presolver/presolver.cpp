#include "cxxopts.hpp"
#include <cstdint>
#include <iostream>
#include <string>

// HACK: Need to write a header file
void preplace(unsigned N, bool count_solutions);

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

    preplace(boardsize, true);

    return 0;
}
