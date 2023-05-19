#pragma once

#include <array>
#include <cassert>

namespace queens {
class Symmetry {
    private:
        static constexpr std::array NAMES = {"<INVALID>", "ROTATE", "POINT", "NONE"};

    public:
        static constexpr unsigned NONE = 3;
        static constexpr unsigned POINT = 2;
        static constexpr unsigned ROTATE = 1;

    private:
        unsigned m_val;

    public:
        Symmetry(unsigned val) : m_val(val) { assert(val > 0 && val < 4); }
        ~Symmetry() = default;
        operator unsigned() const { return m_val; }
        operator char const *() const { return NAMES[m_val]; }

    public:
        unsigned weight() const { return 1 << m_val; }
};

static const std::array<Symmetry, 3> ALL_SYMMETRIES{Symmetry{Symmetry::NONE}, Symmetry{Symmetry::POINT},
                                                    Symmetry{Symmetry::ROTATE}};
} // namespace queens
