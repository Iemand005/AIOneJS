#include <napi.h>
#include <AIOne>

Napi::String Hello(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "Hello from C++");
}

ModelFactory factory;

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, Hello));
	// factory.
    return exports;
}

Napi::Object LoadModel(Napi::String path) {
    LLModelOptions options;
    factory.loadLLM(path.Utf8Value(), options);
}

NODE_API_MODULE(mylibrary, Init)