cmake_minimum_required(VERSION 3.20)
project(AIOneJS LANGUAGES C CXX)

add_subdirectory(
    "../AIOne"
    "${CMAKE_CURRENT_BINARY_DIR}/AIOne"
)

add_library(AIOneNode MODULE
    bindings/node/NodeAddon.cpp
)

target_include_directories(AIOneNode PRIVATE
    ${CMAKE_JS_INC}
    ${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api
)

target_include_directories(AIOneNode PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api"
    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-api-headers/include"
)

target_link_libraries(AIOneNode PRIVATE
    AIOne::Core
    ${CMAKE_JS_LIB}
)

set_target_properties(AIOneNode PROPERTIES
    PREFIX ""
    SUFFIX ".node"
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)