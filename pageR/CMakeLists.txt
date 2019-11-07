cmake_minimum_required(VERSION 3.14)
project(pageR C)

set(CMAKE_C_STANDARD 99)

add_executable(pageR step1.c step2.c step2mmap.c step3mmap.c step3.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -fopenmp")