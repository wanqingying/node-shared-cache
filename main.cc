#include "napi.h"
#include "node-share-cache.cc"


Napi::String MethodHello(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  return Napi::String::New(env, " world v23 ");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, MethodHello));
  NodeShareCache::Init(env, exports);
  return exports;
}

NODE_API_MODULE(hello, Init)
