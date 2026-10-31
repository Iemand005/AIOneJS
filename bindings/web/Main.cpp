#include <emscripten/bind.h>

#include "ModelSession.hpp"

namespace {

ModelSession session;

std::string Hello() {
    return "Hello from C++";
}

void LoadModel(const std::string& path) {
    session.loadModel(path);
}

std::string Generate(const std::string& prompt) {
    return session.generate(prompt);
}

std::string GenerateWithOptions(
    const std::string& prompt,
    unsigned int maxTokens
) {
    return session.generate(prompt, maxTokens);
}

void UnloadModel() {
    session.unloadModel();
}

bool IsLoaded() {
    return session.isLoaded();
}

}

EMSCRIPTEN_BINDINGS(ai_one) {
    emscripten::function("hello", &Hello);
    emscripten::function("loadModel", &LoadModel);
    emscripten::function("generate", &Generate);
    emscripten::function("generateWithOptions", &GenerateWithOptions);
    emscripten::function("unloadModel", &UnloadModel);
    emscripten::function("isLoaded", &IsLoaded);
}