cmake_minimum_required(VERSION 3.20)
project(AIOneJS LANGUAGES C CXX)

add_subdirectory(../AIOne )

add_library(AIOneNode MODULE
    bindings/node/NodeAddon.cpp
)

target_include_directories(AIOneNode PRIVATE
    ${CMAKE_JS_INC}
    ${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api
)

target_link_libraries(AIOneNode PRIVATE
    MyLibrary
    ${CMAKE_JS_LIB}
)

set_target_properties(AIOneNode PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)