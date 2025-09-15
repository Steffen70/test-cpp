# Minimum CMake version and project definition
cmake_minimum_required(VERSION 3.22)

project(superbuild LANGUAGES C)

add_subdirectory(stack)

add_subdirectory(test_stack)
