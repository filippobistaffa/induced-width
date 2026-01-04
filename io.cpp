#include "io.hpp"

#include <fstream>  // ifstream, getline

// fmt library
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

#include <archive.h>
#include <archive_entry.h>
#include <stdexcept>

std::string read_from_tar(const std::string& instance) {

    struct archive* a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_filter_all(a);

    if (archive_read_open_filename(a, instance.c_str(), 10240) != ARCHIVE_OK) {
        throw std::runtime_error(archive_error_string(a));
    }

    struct archive_entry* entry;
    std::string content;

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if (archive_entry_filetype(entry) == AE_IFREG) {
            const size_t size = archive_entry_size(entry);
            content.resize(size);
            archive_read_data(a, content.data(), size);
            break;
        }
        archive_read_data_skip(a);
    }

    archive_read_free(a);

    if (content.empty()) {
        throw std::runtime_error("No regular file found in archive");
    }

    return content;
}

std::vector<std::vector<weight>> read_adj(const std::string& instance) {

    std::string str;

    if (instance.ends_with(".tar.gz") || instance.ends_with(".tar.xz") || instance.ends_with(".tar")) {
        str = read_from_tar(instance);
    } else {
        std::ifstream f(instance);
        if (!f) {
            throw std::runtime_error("Cannot open file: " + instance);
        }
        std::getline(f, str);
    }

    auto tokens = tokenize<std::size_t>(str);
    std::size_t n = sqrt(tokens.size());
    std::vector<std::vector<weight>> adj;

    for (std::size_t i = 0; i < n; ++i) {
        std::vector<weight> row;
        row.assign(tokens.begin() + i * n, tokens.begin() + (i + 1) * n);
        adj.push_back(row);
    }

    return adj;
}
