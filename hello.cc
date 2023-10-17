#include "napi.h"
#include "boost-share-map.cc"

// write a git commit with message "v45"
// git commit -am "v45"

static BoostShareMap *bsm = nullptr;

Napi::String Method(const Napi::CallbackInfo &info)
{
  // print v1
  std::cout << "v1" << std::endl;
  bsm->insert("key1", "value1");
  bsm->insert("key2", "value999");

  // bsm->print();
  bsm->print();

  std::cout << *bsm->get("key1") << std::endl;

  Napi::Env env = info.Env();
  return Napi::String::New(env, "world v45");
}

Napi::String MethodGet(const Napi::CallbackInfo &info)
{

  std::string arg0 = info[0].As<Napi::String>().Utf8Value();

  std::string *res = bsm->get(arg0);

  // if null
  if (res == nullptr)
  {
    Napi::Env env = info.Env();
    return Napi::String::New(env, "null");
  }

  Napi::Env env = info.Env();
  return Napi::String::New(env, *res);
}

Napi::String MethodSet(const Napi::CallbackInfo &info)
{

  std::string arg0 = info[0].As<Napi::String>().Utf8Value();
  std::string arg1 = info[1].As<Napi::String>().Utf8Value();

  bsm->insert(arg0, arg1);

  Napi::Env env = info.Env();
  return Napi::String::New(env, "ok");
}

Napi::Value MethodGetAnyType(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  return Napi::Number::New(env, 589);
}

Napi::String MethodOpen(const Napi::CallbackInfo &info)
{
  bool arg0 = info[0].As<Napi::Boolean>().Value();

  // bsm= new BoostShareMap("HighscoreV2", 4096*100, true);
  // open
  bsm = new BoostShareMap("HighscoreV2", 4096 * 100, arg0);

  Napi::Env env = info.Env();
  return Napi::String::New(env, "ok");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));

  exports.Set(Napi::String::New(env, "get"),
              Napi::Function::New(env, MethodGet));

  exports.Set(Napi::String::New(env, "set"),
              Napi::Function::New(env, MethodSet));

  exports.Set(Napi::String::New(env, "open"),
              Napi::Function::New(env, MethodOpen));

  exports.Set(Napi::String::New(env, "getAnyType"), Napi::Function::New(env, MethodGetAnyType));
  return exports;
}

NODE_API_MODULE(hello, Init)
