#include "main.hpp"

#include <filesystem>   // std::filesystem::exists
#include <chrono>       // time measurement
#include <unistd.h>     // getopt
#include <string>       // string
#include <fstream>      // std::ofstream

#include "io.hpp"
#include "log.hpp"
#include "order.hpp"
#include "timer.hpp"

// json library
#include "json.hpp"

// fmt library
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>

static inline void print_usage(const char *bin) {

    fmt::print(stderr, "Usage: {} [-h] [-o wmf|mf|miw|md|random] ", bin);
    fmt::print(stderr, "[-t unique|random] [-j json] -f instance\n");
}

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();
    int ord_heur = O_WEIGHTED_MIN_FILL;
    int tie_heur = T_UNIQUENESS;
    std::string instance;
    size_t seed = 0;
    int opt;

    // export results to json
    std::string json_path;
    nlohmann::json json;

    while ((opt = getopt(argc, argv, "f:o:t:s:j:h")) != -1) {
        switch (opt) {
            case 'f':
                if (std::filesystem::exists(optarg)) {
                    instance = optarg;
                } else {
                    fmt::print(stderr, "{}: file not found -- '{}'\n", argv[0], optarg);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
                continue;
            case 'j':
                json_path = optarg;
                continue;
            case 's':
                seed = atoi(optarg);
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
                    fmt::print(stderr, "{}: invalid order heuristic -- '{}'\n", argv[0], optarg);
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

    if (instance.empty()) {
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
    const auto variables = adj.size();
    log_fmt("Number of variables", variables);

    srand(seed);
    log_fmt("Seed", seed);

    // compute order according to the chosen heuristic
    std::string ord_heur_names[] = { "WEIGHTED-MIN-FILL", "MIN-FILL", "MIN-INDUCED-WIDTH", "MIN-DEGREE" };
    std::string tie_heur_names[] = { "MIN-UNIQUENESS", "RANDOM" };
    std::vector<std::size_t> order;
    log_fmt("Variable order heuristic", ord_heur_names[ord_heur]);
    log_fmt("Tie-breaking heuristic", tie_heur_names[tie_heur]);
    log_line();
    order = greedy_order(adj, ord_heur, tie_heur);
    log_line();
    const float runtime_order = ELAPSED(start);
    log_fmt("Order computation runtime", fmt::format("{:.2f}s", runtime_order));
    log_line();

    // compute induced width
    //fmt::print("Order: {}\n", order);
    std::reverse(order.begin(), order.end());
    std::vector<std::size_t> pos(order.size());
    for (std::size_t i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    auto iw = induced_width(adj, order);
    const float runtime_iw = ELAPSED(start);
    log_line();
    log_fmt("Induced width", iw);
    log_fmt("Induced width computation runtime", fmt::format("{:.2f}s", runtime_iw));
    log_line();

    // export json if necessary
    #define JSON_FIELD(VAR) {#VAR, VAR}
    if (!json_path.empty()) {
        json["input"] = {
            JSON_FIELD(instance),
            JSON_FIELD(variables),
            JSON_FIELD(seed),
            JSON_FIELD(ord_heur),
        };
        json["output"] = {
            JSON_FIELD(iw),
            JSON_FIELD(runtime_iw),
            JSON_FIELD(runtime_order),
        };
        std::ofstream of(json_path);
        of << json.dump(2);
        of.close();
    }

    return EXIT_SUCCESS;
}
