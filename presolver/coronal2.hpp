#pragma once

#include "board.hpp"

#include "symmetry.hpp"
#include <array>
#include <functional>
#include <vector>

/**
 * Callback for the result of the preplacer, first parameter is the preplaced Board, second the direction of symmetry.
 */
using PreplaceCallback = void(queens::Board const &, queens::Symmetry::Direction);

/**
 * @brief Run preplacer from q27 project
 * @param N boardsize
 * @param callback Callback to further handle each computed preplacement
 */
void preplace(unsigned N, std::function<PreplaceCallback> callback);
