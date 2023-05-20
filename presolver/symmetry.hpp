#pragma once

#include <array>

namespace queens {
class Symmetry {
    private:
        static constexpr std::array NAMES = {"ROTATE", "POINT", "NONE"};

    public:
        enum class Direction {
            NONE = 2,
            POINT = 1,
            ROTATE = 0,
        };

    private:
        Direction m_val;

    public:
        Symmetry(Direction val) : m_val{val} {};
        ~Symmetry() = default;
        operator unsigned() const { return static_cast<unsigned>(m_val); }
        operator char const *() const { return NAMES[*this]; }

    public:
        unsigned weight() const { return 1 << (static_cast<unsigned>(m_val) + 1); }
};

static const std::array<Symmetry, 3> ALL_SYMMETRIES{
    Symmetry{Symmetry::Direction::NONE}, Symmetry{Symmetry::Direction::POINT}, Symmetry{Symmetry::Direction::ROTATE}};
} // namespace queens
