#include "io.hpp"

// fmt library
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/ranges.h>

#define PRECISION 2

void print_adj(std::vector<std::vector<weight>> const &adj) {

        const std::size_t n_vars = adj.size();
        const int var = 1 + floor(log10(n_vars - 1));
        const int column = std::max(PRECISION + 2, var);

        fmt::print("{1: >{0}}", var + 3, "");

        for (std::size_t i = 0; i < n_vars; ++i) {
                fmt::print("{0: >{1}} ", i, column);
        }

        fmt::print("\n{1: >{0}}+", var + 1, "");
        fmt::print("{1:->{0}}\n", (column + 1) * n_vars, "");

        for (std::size_t i = 0; i < n_vars; ++i) {
                fmt::print("{0: >{1}} | ", i, var);
                for (std::size_t j = 0; j < n_vars; ++j) {
                        fmt::print("{0:.{1}f} ", adj[i][j], PRECISION);
                }
                fmt::print("\n");
        }
}

std::vector<std::vector<weight>> read_adj(const char *instance) {

    std::vector<std::vector<weight>> adj;
    return adj;
}
