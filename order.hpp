#ifndef ORDER_HPP_
#define ORDER_HPP_

#include "types.hpp"

enum order_heur {
        O_WEIGHTED_MIN_FILL,
        O_MIN_FILL,
        O_MIN_INDUCED_WIDTH,
        O_MIN_DEGREE,
        O_RANDOM
};

enum tie_heur {
        T_UNIQUENESS,
        T_RANDOM
};

#include <limits>   // std::numeric_limits
#include <vector>

template <typename T>
struct compare_vec {
        compare_vec(std::vector<T> const &vec) : vec(vec) {};
        bool operator()(std::size_t const &x, std::size_t const &y) {
                if constexpr (std::is_integral_v<T>) {
                        return vec[x] < vec[y];
                } else if (std::is_floating_point_v<T>) {
                        return vec[x] < vec[y] - std::numeric_limits<weight>::epsilon();
                }
        }
        std::vector<T> vec;
};

std::vector<std::size_t> greedy_order(std::vector<std::vector<weight>> const &adj, int order_heur = O_WEIGHTED_MIN_FILL, int tie_heur = T_UNIQUENESS);

std::size_t induced_width(std::vector<std::vector<weight>> const &adj, std::vector<std::size_t> const &order);

void export_order(std::vector<std::size_t> const &order, std::vector<std::size_t> const &domains, const char *output);

#endif /* ORDER_HPP_ */
