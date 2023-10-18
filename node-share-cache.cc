
#include <napi.h>
#include "boost-share-cache.cc"

class NodeShareCache : public Napi::ObjectWrap<NodeShareCache>
{

private:
    double value_ = 10;
    BoostShareCache *bsc;

public:
    NodeShareCache(const Napi::CallbackInfo &info)
        : Napi::ObjectWrap<NodeShareCache>(info)
    {
        Napi::Env env = info.Env();

        int length = info.Length();
        if (length <= 0 || !info[0].IsString())
        {
            Napi::TypeError::New(env, " arg0 string expected").ThrowAsJavaScriptException();
            return;
        }
        std::string name = info[0].As<Napi::String>().Utf8Value();

        long size = 1024 * 4 * 10;
        if (length > 1 && info[1].IsNumber())
        {
            Napi::Number sizeNum = info[1].As<Napi::Number>();
            size = sizeNum.Int32Value();
        }
        bool renew = false;
        if (length > 2 && info[2].IsBoolean())
        {
            Napi::Boolean renewBool = info[2].As<Napi::Boolean>();
            renew = renewBool.Value();
        }
        // cache
        bsc = new BoostShareCache(name, size, renew);
    }

private:
    Napi::Value res(const Napi::CallbackInfo &info, TDataType &v)
    {
        Napi::Env env = info.Env();

        if (auto bool_ptr = std::get_if<bool>(&v))
        {
            return Napi::Boolean::New(env, *bool_ptr);
        }
        else if (auto double_ptr = std::get_if<double>(&v))
        {
            return Napi::Number::New(env, *double_ptr);
        }
        else if (auto str_ptr = std::get_if<std::string>(&v))
        {
            return Napi::String::New(env, *str_ptr);
        }
        else if (auto shm_str_ptr = std::get_if<ShmString>(&v))
        {
            return Napi::String::New(env, shm_str_ptr->c_str());
        }
        else
        {
            return Napi::String::New(env, "not support type");
        }
    }
    Napi::Value get(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 0 || !info[0].IsString())
        {
            Napi::TypeError::New(info.Env(), " arg0 string expected").ThrowAsJavaScriptException();
            return Napi::String::New(info.Env(), "null");
        }
        std::string arg0 = info[0].As<Napi::String>().Utf8Value();
        auto *res = bsc->get(arg0);
        if (res == nullptr)
        {
            return info.Env().Undefined();
        }
        return this->res(info, *res);
    }
    Napi::Value set(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 1 || !info[0].IsString() || !info[1].IsString())
        {
            Napi::TypeError::New(info.Env(), " arg0 string expected").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(), 0);
        }

        std::string arg0 = info[0].As<Napi::String>().Utf8Value();
        std::string arg1 = info[1].As<Napi::String>().Utf8Value();

        bsc->insert(arg0, arg1);

        return Napi::Number::New(info.Env(), 1);
    }
    Napi::Value setMaxSize(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 0 || !info[0].IsNumber())
        {
            Napi::TypeError::New(info.Env(), " arg0 number expected").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(), 0);
        }

        Napi::Number sizeNum = info[0].As<Napi::Number>();
        long size = sizeNum.Int32Value();
        this->bsc->setMaxSize(size);
        return Napi::Number::New(info.Env(), 1);
    }

    Napi::Value setMaxAge(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 0 || !info[0].IsNumber())
        {
            Napi::TypeError::New(info.Env(), " arg0 number expected").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(), 0);
        }

        Napi::Number sizeNum = info[0].As<Napi::Number>();
        long size = sizeNum.Int32Value();
        this->bsc->setMaxAge(size);
        return Napi::Number::New(info.Env(), 1);
    }
    Napi::Value setLogLevel(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 0 || !info[0].IsNumber())
        {
            Napi::TypeError::New(info.Env(), " arg0 number expected").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(), 0);
        }

        Napi::Number sizeNum = info[0].As<Napi::Number>();
        int lv = sizeNum.Int32Value();
        this->bsc->setLogLevel(static_cast<nnd::ShmLog::ELevel>(lv));
        return Napi::Number::New(info.Env(), 1);
    }

    Napi::Value setLock(const Napi::CallbackInfo &info)
    {
        int length = info.Length();
        if (length <= 0 || !info[0].IsBoolean())
        {
            Napi::TypeError::New(info.Env(), " arg0 bool expected").ThrowAsJavaScriptException();
            return Napi::Number::New(info.Env(), 0);
        }

        Napi::Boolean enable = info[0].As<Napi::Boolean>();
        this->bsc->setLock(enable.Value());
        return Napi::Number::New(info.Env(), 1);
    }

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports)
    {
        Napi::Function func =
            DefineClass(env,
                        "NodeShareCache",
                        {InstanceMethod("get", &NodeShareCache::get),
                         InstanceMethod("set", &NodeShareCache::set),
                         InstanceMethod("setLogLevel", &NodeShareCache::setLogLevel),
                         InstanceMethod("setMaxAge", &NodeShareCache::setMaxAge),
                         InstanceMethod("setLock", &NodeShareCache::setLock),
                         InstanceMethod("setMaxSize", &NodeShareCache::setMaxSize)});

        Napi::FunctionReference *constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        env.SetInstanceData(constructor);

        exports.Set("NodeShareCache", func);
        return exports;
    }
};