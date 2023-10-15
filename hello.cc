#include "napi.h"
#include "boost-share-map.cc"

// write a git commit with message "v45"
// git commit -am "v45"

Napi::String Method(const Napi::CallbackInfo &info)
{
  // print v1
  std::cout << "v1" << std::endl;
  BoostShareMap *bsm = new BoostShareMap("HighscoreV2", 1024, true);
  bsm->insert("key1", "value1");
  bsm->insert("key2", "value2");

  // bsm->print();
  bsm->print();

  std::cout << *bsm->get("key1") << std::endl;

  Napi::Env env = info.Env();
  return Napi::String::New(env, "world v45");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init)
