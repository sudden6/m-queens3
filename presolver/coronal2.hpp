#pragma once

#include "board.hpp"

#include <array>
#include <vector>
#include "symmetry.hpp"

// Index is the queens::Symmetry::Direction
using Preplacements = std::array<std::vector<queens::Board>, queens::ALL_SYMMETRIES.size()>;

/**
 * @brief Run preplacer from q27 project
 * @param N boardsize
 * @param count_solutions if true evaluate and count all computed preplacements
 */
Preplacements preplace(unsigned N);
