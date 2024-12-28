cmake_minimum_required(VERSION 3.30)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        prng/mtwister.c
        prng/mtwister.h
        prng/boxmuller.c
        prng/boxmuller.h)
