cmake_minimum_required(VERSION 3.25.0)

project(logging VERSION 1.3.0)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/logging.c
)

# Public interface - what users of this library get
target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc         # Public headers
)

# Auto-generate version from CMake project version
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        LOGGING_VERSION="${PROJECT_VERSION}"
        LOGGING_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        LOGGING_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        LOGGING_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

# Library just provides the interface - no policy decisions


