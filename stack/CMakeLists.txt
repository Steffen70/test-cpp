# Minimum CMake version and project definition
cmake_minimum_required(VERSION 3.22)

# Resolve the directory name.
get_filename_component(PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_DIR_NAME} LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES
        CONFIGURE_DEPENDS
        src/*.c
)

add_library(stack STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
