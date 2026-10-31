#include <napi.h>

#include <memory>
#include <string>

#include "LLModel.hpp"
#include "ModelFactory.hpp"

namespace {

ModelFactory factory;
LLModelPtr model;
std::shared_ptr<TextContext> context;

} // namespace

Napi::String Hello(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "Hello from C++");
}

Napi::Value LoadModel(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a model path").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto path = info[0].As<Napi::String>().Utf8Value();

    try {
        // Make sure the GGUFs backend is registered before picking a device.
        factory.loadBackends();

        LLModelOptions options;
        options.device = PreferredDevice::DGPU; // VRAM; falls back if no GPU
        options.offloadLayers = -1;             // all layers on the device
        options.useMmap = false;

        context.reset();
        model = factory.loadLLM(path, options);
        context = model->createContext();
    } catch (std::exception const& e) {
        context.reset();
        model.reset();
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Undefined();
}

Napi::Value Generate(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (!model || !context) {
        Napi::Error::New(env, "No model loaded; call loadModel() first")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a prompt string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto prompt = info[0].As<Napi::String>().Utf8Value();

    TextGenOptions options;
    if (info.Length() > 1 && info[1].IsNumber())
        options.maxTokens = info[1].As<Napi::Number>().Uint32Value();

    try {
        auto result = model->complete(context, prompt, options);
        return Napi::String::New(env, result.output.content);
    } catch (std::exception const& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Value UnloadModel(const Napi::CallbackInfo& info) {
    // Context must die before the model it points into.
    context.reset();
    model.reset();
    return info.Env().Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("hello", Napi::Function::New(env, Hello));
    exports.Set("loadModel", Napi::Function::New(env, LoadModel));
    exports.Set("generate", Napi::Function::New(env, Generate));
    exports.Set("unloadModel", Napi::Function::New(env, UnloadModel));
    return exports;
}

NODE_API_MODULE(mylibrary, Init)
