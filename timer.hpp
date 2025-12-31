#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

#define DIFFERENCE(START) \
    (std::chrono::high_resolution_clock::now() - START)

#define ELAPSED(START) \
    (1E-6f * std::chrono::duration_cast<std::chrono::microseconds>(DIFFERENCE(START)).count())

#endif
