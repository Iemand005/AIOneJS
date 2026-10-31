#include <napi.h>
#include <AIOne>

Napi::String Hello(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "Hello from C++");
}

ModelFactory factory;



Napi::Value LoadModel(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a model path").ThrowAsJavaScriptException();
        return env.Null();
    }

    LLModelOptions options;
    auto model = factory.loadLLM(info[0].As<Napi::String>().Utf8Value(), options);

    // Return or wrap model here
    return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, Hello));
    exports.Set("loadModel", Napi::Function::New(env, LoadModel));
	// factory.
    return exports;
}


NODE_API_MODULE(mylibrary, Init)