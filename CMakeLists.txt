cmake_minimum_required(VERSION 3.22)
set(NAME induced-width)
project(${NAME})
set(CMAKE_CXX_STANDARD 20)

set(PEDANTIC_COMPILE_FLAGS
    -Wall
    -Wextra
    -pedantic
    -pedantic-errors
)

set(EXCLUDED_WARNINGS
    #-Wno-sign-compare
    #-Wno-unused-result
)

set(OPTIMIZATION_COMPILE_FLAGS
    -Ofast
    -march=native
    -funroll-loops
    -funsafe-loop-optimizations
    -falign-functions=16
    -falign-loops=16
    -fopenmp
)

set(SOURCES
    main.cpp
    order.cpp
    io.cpp
)

include_directories(fmt/include)

add_executable(${NAME} ${SOURCES})
target_compile_options(${NAME} PRIVATE ${PEDANTIC_COMPILE_FLAGS} ${EXCLUDED_WARNINGS} ${OPTIMIZATION_COMPILE_FLAGS})
