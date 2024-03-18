#ifndef IO_HPP_
#define IO_HPP_

#include "types.hpp"
#include <vector>

void print_adj(std::vector<std::vector<weight>> const &adj);

std::vector<std::vector<weight>> read_adj(const char *instance);

#endif /* IO_HPP_ */
