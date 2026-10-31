#include <napi.h>

#include <exception>
#include <string>

#include "ModelSession.hpp"

namespace {

AIOne::ModelSession session;

}

Napi::String Hello(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "Hello from C++");
}

Napi::Value LoadModel(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a model path").ThrowAsJavaScriptException();
        return env.Null();
    }

    try {
        session.loadModel(info[0].As<Napi::String>().Utf8Value());
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Undefined();
}

Napi::Value Generate(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (!session.isLoaded()) {
        Napi::Error::New(env, "No model loaded; call loadModel() first")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a prompt string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto prompt = info[0].As<Napi::String>().Utf8Value();
    unsigned int maxTokens = 128;

    if (info.Length() > 1 && info[1].IsNumber())
        maxTokens = info[1].As<Napi::Number>().Uint32Value();

    try {
        auto result = session.generate(prompt, maxTokens);
        return Napi::String::New(env, result);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Value UnloadModel(const Napi::CallbackInfo& info) {
    session.unloadModel();
    return info.Env().Undefined();
}

Napi::Value IsLoaded(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), session.isLoaded());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("hello", Napi::Function::New(env, Hello));
    exports.Set("loadModel", Napi::Function::New(env, LoadModel));
    exports.Set("generate", Napi::Function::New(env, Generate));
    exports.Set("unloadModel", Napi::Function::New(env, UnloadModel));
    exports.Set("isLoaded", Napi::Function::New(env, IsLoaded));
    return exports;
}

NODE_API_MODULE(mylibrary, Init)