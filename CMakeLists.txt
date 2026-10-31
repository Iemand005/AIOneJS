cmake_minimum_required(VERSION 3.20)
project(AIOneJS LANGUAGES C CXX)

set(LLAMA_BUILD_COMMON ON CACHE BOOL "" FORCE)

# 1. Fetch cmake-js runtime properties if they aren't injected automatically
if(NOT DEFINED CMAKE_JS_INC)
    execute_process(COMMAND cmake-js compile --print-configure-env
        OUTPUT_VARIABLE CMAKE_JS_ENV
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    # Note: If running via 'npx cmake-js compile', cmake-js injects these variables automatically.
endif()

add_subdirectory(
    "../AIOne"
    "${CMAKE_CURRENT_BINARY_DIR}/AIOne"
)

# 2. Extract the actual include path and source file from node-addon-api
execute_process(COMMAND node -p "require('node-addon-api').include"
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Remove quotes if returned by the node evaluation
string(REPLACE "\"" "" NODE_ADDON_API_DIR "${NODE_ADDON_API_DIR}")

# 3. Add your addon module, INCLUDING the mandatory node-addon-api source bridge file
add_library(AIOneNode MODULE
    bindings/node/NodeAddon.cpp
    "${NODE_ADDON_API_DIR}/../src/node_api.cc"  # <-- This is the missing link!
)

# 4. Correct your include directories
target_include_directories(AIOneNode PRIVATE
    ${CMAKE_JS_INC}
    ${NODE_ADDON_API_DIR}
)

# 5. Link against your core library and the node runtime library
target_link_libraries(AIOneNode PRIVATE
    AIOne::Core
    ${CMAKE_JS_LIB}
)

target_compile_definitions(AIOneNode PRIVATE
    NODE_GYP_MODULE_NAME=AIOneNode
    NAPI_VERSION=8
    NAPI_DISABLE_CPP_EXCEPTIONS # Highly recommended for node-addon-api performance
)

set_target_properties(AIOneNode PROPERTIES
    PREFIX ""
    SUFFIX ".node"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 6. Windows-specific: Generate node.lib target out of the definition file if cmake-js provides it
if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
    execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
