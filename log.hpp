#ifndef LOG_HPP_
#define LOG_HPP_

// fmt library
#include <fmt/core.h>

#define TOTAL_WIDTH 79
#define COLUMN_WIDTH ((TOTAL_WIDTH - 7) / 2)

#define log_fmt(...) _log_fmt(__VA_ARGS__, "")
#define _log_fmt(A, B, C, ...) log_string(A, fmt::format("{}", B), C)

static float log_progress;

inline void log_title(std::string title, size_t add_space = 0) {

    fmt::print("| {1:^{0}} |\n", TOTAL_WIDTH - 4 + add_space, title);
    std::fflush(nullptr);
}

inline void log_line() {

    fmt::print("+{1:->{0}}+{1:->{0}}+\n", COLUMN_WIDTH + 2, "");
    std::fflush(nullptr);
}

inline void log_string(std::string name, std::string val, std::string param = "", size_t add_space = 0) {

    fmt::print("| ");
    const size_t par_space = param.length() + param.length() ? 5 : 0;
    if (name.length() > COLUMN_WIDTH - par_space + add_space) {
        fmt::print("{}...", name.substr(0, COLUMN_WIDTH - 3 - par_space + add_space));
    } else {
        fmt::print("{1:<{0}}", COLUMN_WIDTH - par_space + add_space, name);
    }
    if (param.length()) {
        fmt::print(" (-{})", param);
    }
    fmt::print(" | ");
    if (val.length() > COLUMN_WIDTH) {
        fmt::print("...{}", val.substr(val.length() - COLUMN_WIDTH + 3));
    } else {
        fmt::print("{1:<{0}}", COLUMN_WIDTH, val);
    }
    fmt::print(" |\n");
    std::fflush(nullptr);
}

inline void log_progress_increase(float step, float tot) {

    if (log_progress == tot) {
        return;
    }
    if (log_progress == 0) {
        fmt::print("|");
    }
    const size_t cur_p = (TOTAL_WIDTH - 2) * (log_progress / tot);
    const size_t new_p = (TOTAL_WIDTH - 2) * ((log_progress + step) / tot);
    for (size_t i = cur_p; i < new_p; ++i) {
        fmt::print("·");
        std::fflush(nullptr);
    }
    log_progress += step;
    if (log_progress == tot) {
        fmt::print("|\n");
    }
}

#include "timer.hpp"
static float prev;

template <typename T1, typename T2>
inline void log_elapsed_prev(T1 start, T2 val) {

    float elapsed = ELAPSED(start);
    log_fmt(fmt::format("{:>{}s}", fmt::format("{0}s ({1:+f}s)", elapsed, elapsed - prev), COLUMN_WIDTH), val);
    prev = elapsed;
}

#endif
