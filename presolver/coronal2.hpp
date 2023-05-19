#pragma once

#include "board.hpp"

#include <array>
#include <vector>

// Index is the queens::Symmetry
using Preplacements = std::array<std::vector<queens::Board>, 3>;

/**
 * @brief Run preplacer from q27 project
 * @param N boardsize
 * @param count_solutions if true evaluate and count all computed preplacements
 */
Preplacements preplace(unsigned N, bool count_solutions);
