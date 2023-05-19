add_executable(m-queens3-presolver presolver.cpp coronal2.cpp symmetry.hpp board.hpp)
target_link_libraries(m-queens3-presolver cxxopts::cxxopts)
