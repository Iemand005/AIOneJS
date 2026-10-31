cmake_minimum_required(VERSION 3.20)

project(AIOneJS LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Node.js
find_path(NODE_API_HEADERS_DIR
    node_api.h
    PATHS
        "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-api-headers"
        "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-api-headers/include"
    PATH_SUFFIXES
        node
    REQUIRED
)

find_library(NODE_LIBRARY
    NAMES node
    PATHS
        "C:/Program Files/nodejs"
        "C:/Program Files/nodejs/lib"
        "C:/Program Files/nodejs/lib/node"
    REQUIRED
)

# AIOne
add_subdirectory(
    "../AIOne"
    "${CMAKE_CURRENT_BINARY_DIR}/AIOne"
)

# Node addon
add_library(AIOneNode MODULE
    bindings/node/NodeAddon.cpp
)

target_include_directories(AIOneNode PRIVATE
    "${NODE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api"
)

target_compile_definitions(AIOneNode PRIVATE
    NAPI_VERSION=8
    NODE_GYP_MODULE_NAME=AIOneNode
)

target_link_libraries(AIOneNode PRIVATE
    AIOne::Core
    "${NODE_LIBRARY}"
)

set_target_properties(AIOneNode PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)