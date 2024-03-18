#include "io.hpp"

#include <fstream>  // ifstream, getline

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

template <typename T>
std::vector<T> tokenize(std::string &str, const char *sep = ",") {

        char *dup = strdup(str.c_str());
        char *token = strtok(dup, sep);
        std::vector<T> v;

        while (token != NULL) {
                if constexpr (std::is_integral_v<T>) {
                        v.push_back(atoi(token));
                } else if (std::is_floating_point_v<T>) {
                        v.push_back(atof(token));
                }
                token = strtok(NULL, sep);
        }

        free(dup);
        return v;
}

std::vector<std::vector<weight>> read_adj(const char *instance) {

    std::ifstream f(instance);
    std::string str;
    getline(f, str);
    fmt::print("{}\n", str);
    f.close();
    auto tokens = tokenize<std::size_t>(str);
    fmt::print("{}\n", tokens);
    std::vector<std::vector<weight>> adj;
    return adj;
}
