cmake_minimum_required(VERSION 3.20)
project(AIOneJS LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(
    "../AIOne"
    "${CMAKE_CURRENT_BINARY_DIR}/AIOne"
)

add_library(AIOneNode MODULE
    bindings/node/NodeAddon.cpp
)

target_include_directories(AIOneNode PRIVATE
    "${NODE_API_HEADERS_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api"
)

target_compile_definitions(AIOneNode PRIVATE
    NAPI_VERSION=8
    NODE_GYP_MODULE_NAME=AIOneNode
)

target_link_libraries(AIOneNode PRIVATE
    AIOne::Core
)

set_target_properties(AIOneNode PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)