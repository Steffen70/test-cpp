# Minimum CMake version and project definition
cmake_minimum_required(VERSION 3.22)
get_filename_component(PROJECT_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_DIR_NAME} LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)

# Link protobuf-c runtime (system package provides libprotobuf-c)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUFC REQUIRED libprotobuf-c)

# Proto inputs and generated outputs
get_filename_component(PROTO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../protos" REALPATH)
file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS "${PROTO_ROOT}/*.proto")

set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GEN_DIR}")

# Compute expected outputs
set(GEN_SRCS)
set(GEN_HDRS)
foreach (p IN LISTS PROTO_FILES)
    file(RELATIVE_PATH rel "${PROTO_ROOT}" "${p}")
    get_filename_component(noext "${rel}" NAME_WE)
    get_filename_component(subdir "${rel}" DIRECTORY)
    set(out_dir "${GEN_DIR}/${subdir}")
    list(APPEND GEN_SRCS "${out_dir}/${noext}.pb-c.c")
    list(APPEND GEN_HDRS "${out_dir}/${noext}.pb-c.h")
endforeach ()

# Single invocation of protoc-c for all files
add_custom_command(
        OUTPUT ${GEN_SRCS} ${GEN_HDRS}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GEN_DIR}"
        COMMAND protoc
        --proto_path=${PROTO_ROOT}
        --c_out=${GEN_DIR}
        ${PROTO_FILES}
        DEPENDS ${PROTO_FILES}
        COMMENT "Generating protobuf-c C sources"
        VERBATIM
)

# Add generated sources to the target
add_executable(${PROJECT_NAME}
        ${SRC_FILES}
        src/main.c
        ${GEN_SRCS}
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        _XOPEN_SOURCE=700
)

# Include generated dirs (derive from headers)
set(GEN_INC_DIRS "")
foreach (h IN LISTS GEN_HDRS)
    get_filename_component(d "${h}" DIRECTORY)
    list(APPEND GEN_INC_DIRS "${d}")
endforeach ()
list(REMOVE_DUPLICATES GEN_INC_DIRS)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${GEN_INC_DIRS}
        ${PROTOBUFC_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${PROTOBUFC_LIBRARIES}
        stack
)
