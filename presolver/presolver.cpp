#include "cxxopts.hpp"
#include <cstdint>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    cxxopts::Options options("m-queens3-presolver", "This program generates work units for the m-queens3 solver");
    // clang-format off
    options.add_options()
        ("N,boardsize", "Size of the board [4..30]", cxxopts::value<uint8_t>())
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

    std::cout << "Running with boardsize: " << std::to_string(boardsize) << std::endl;

    return 0;
}
