#ifndef IO_HPP_
#define IO_HPP_

#include "types.hpp"
#include <string>
#include <vector>

void print_adj(std::vector<std::vector<weight>> const &adj);

std::vector<std::vector<weight>> read_adj(const std::string& instance);

#endif /* IO_HPP_ */
