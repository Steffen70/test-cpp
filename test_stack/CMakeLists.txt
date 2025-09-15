# Minimum CMake version and project definition
cmake_minimum_required(VERSION 3.22)
get_filename_component(PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_DIR_NAME} LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)

# Paths to generated protobuf-c sources (collect all, recursively)
set(GEN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/generated")

# All generated C sources and headers
file(GLOB_RECURSE GEN_SRCS "${GEN_ROOT}/*.pb-c.c")
file(GLOB_RECURSE GEN_HDRS "${GEN_ROOT}/*.pb-c.h")

# Derive unique include directories from generated headers (and sources as fallback)
set(GEN_INC_DIRS "")
foreach(hdr IN LISTS GEN_HDRS)
    get_filename_component(dir "${hdr}" DIRECTORY)
    list(APPEND GEN_INC_DIRS "${dir}")
endforeach()
list(REMOVE_DUPLICATES GEN_INC_DIRS)

# Executable with all generated sources
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    src/main.c
    ${GEN_SRCS}
)

# Include all generated directories
target_include_directories(${PROJECT_NAME} PRIVATE ${GEN_INC_DIRS})

# Link protobuf-c runtime (system package provides libprotobuf-c)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUFC REQUIRED libprotobuf-c)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${PROTOBUFC_LIBRARIES}
    stack
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROTOBUFC_INCLUDE_DIRS}
)
