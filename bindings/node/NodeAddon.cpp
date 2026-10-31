#include <napi.h>
#include <AIOne>

Napi::String Hello(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "Hello from C++");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, Hello));
	AIOne::ModelFactory
    return exports;
}

NODE_API_MODULE(mylibrary, Init)