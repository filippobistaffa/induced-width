#include "main.hpp"

#include <chrono>       // time measurement
#include <string.h>     // strcmp
#include <unistd.h>     // getopt

#include "io.hpp"
#include "log.hpp"
#include "order.hpp"

// fmt library
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>

static inline void print_usage(const char *bin) {

    fmt::print(stderr, "Usage: {} [-h] [-a bub|brz|hop] [-i bound] [-o wmf|mf|miw|md|random|*.pt] ", bin);
    fmt::print(stderr, "[-t unique|random] [-s seed] [-O order] [-r] -f instance\n");
}

static inline bool exists(const char *filename) {

    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    } else {
        fclose(file);
        return true;
    }
}

int main(int argc, char *argv[]) {

    int ord_heur = O_WEIGHTED_MIN_FILL;
    int tie_heur = T_UNIQUENESS;
    char *instance = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "f:o:t:s:O:hr")) != -1) {
        switch (opt) {
            case 'f':
                if (exists(optarg)) {
                    instance = optarg;
                } else {
                    fmt::print(stderr, "{}: file not found -- '{}'\n", argv[0], optarg);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
                continue;
            case 'o':
                if (strcmp(optarg, "wmf") == 0) {
                    ord_heur = O_WEIGHTED_MIN_FILL;
                } else if (strcmp(optarg, "mf") == 0) {
                    ord_heur = O_MIN_FILL;
                } else if (strcmp(optarg, "miw") == 0) {
                    ord_heur = O_MIN_INDUCED_WIDTH;
                } else if (strcmp(optarg, "md") == 0) {
                    ord_heur = O_MIN_DEGREE;
                } else if (strcmp(optarg, "random") == 0) {
                    ord_heur = O_RANDOM;
                } else {
                    fmt::print(stderr, "{}: file not found -- '{}'\n", argv[0], optarg);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
                continue;
            case 't':
                if (strcmp(optarg, "unique") == 0) {
                    tie_heur = T_UNIQUENESS;
                } else if (strcmp(optarg, "random") == 0) {
                    tie_heur = T_RANDOM;
                } else {
                    fmt::print(stderr, "{}: tie-breaking heuristic not valid -- '{}'\n", argv[0], optarg);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
                continue;
            case 'h':
            default :
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!instance) {
        fmt::print(stderr, "{}: instance not specified!\n", argv[0]);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    log_line();
    log_title("Calculate the induced width of a constraint network");
    log_title("https://github.com/filippobistaffa/induced-width");
    log_line();

    // read input file and construct adjacency matrix
    log_fmt("Instance", instance);
    auto adj = read_adj(instance);
    //print_adj(adj);
    log_fmt("Number of variables", adj.size());

    // compute order according to the chosen heuristic
    std::string ord_heur_names[] = { "WEIGHTED-MIN-FILL", "MIN-FILL", "MIN-INDUCED-WIDTH", "MIN-DEGREE" };
    std::string tie_heur_names[] = { "MIN-UNIQUENESS", "RANDOM" };
    std::vector<size_t> order;
    auto start_t = std::chrono::high_resolution_clock::now();
    log_fmt("Variable order heuristic", ord_heur_names[ord_heur]);
    log_fmt("Tie-breaking heuristic", tie_heur_names[tie_heur]);
    log_line();
    order = greedy_order(adj, ord_heur, tie_heur);
    std::chrono::duration<double> runtime = std::chrono::high_resolution_clock::now() - start_t;
    log_line();
    log_fmt("Order computation runtime", fmt::format("{:%T}", runtime));
    log_line();

    // compute induced width
    //fmt::print("Order: {}\n", order);
    reverse(order.begin(), order.end());
    std::vector<size_t> pos(order.size());
    for (size_t i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    log_line();
    start_t = std::chrono::high_resolution_clock::now();
    auto iw = induced_width(adj, order);
    runtime = std::chrono::high_resolution_clock::now() - start_t;
    log_line();
    log_fmt("Induced width", iw);
    log_fmt("Induced width computation runtime", fmt::format("{:%T}", runtime));
    log_line();

    return EXIT_SUCCESS;
}
