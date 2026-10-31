#include <emscripten/bind.h>

#include <ModelSession.hpp>

namespace {

AIOne::ModelSession session;

}

EMSCRIPTEN_BINDINGS(ai_one) {
    emscripten::function("hello", [] {
        return std::string("Hello from C++");
    });

    emscripten::function("loadModel", &AIOne::ModelSession::loadModel);
    emscripten::function("generate", &AIOne::ModelSession::generate);
    emscripten::function("unloadModel", &AIOne::ModelSession::unloadModel);
    emscripten::function("isLoaded", &AIOne::ModelSession::isLoaded);
}