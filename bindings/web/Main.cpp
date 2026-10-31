#include <emscripten/bind.h>
#include <string>

#include <AIOne>

std::string version() {
    return "AIOne Web 1.0";
}

std::string echo(const std::string& input) {
    return input;
}

EMSCRIPTEN_BINDINGS(ai_one) {
    emscripten::function("version", &version);
    emscripten::function("echo", &echo);
}