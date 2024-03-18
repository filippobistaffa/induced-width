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
        log_fmt("Instance", instance);
        int inst_type;

        auto adj = read_adj(instance);

        return EXIT_SUCCESS;
}
