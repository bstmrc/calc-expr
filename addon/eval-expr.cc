#include <napi.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
HINSTANCE lib = nullptr;
#else
#include <dlfcn.h>
void* lib = nullptr;
#endif

typedef struct {
  int ok;
  double result;
  const char* error;
} EvalResult;

typedef EvalResult* (*EvalFn)(const char*, const char*);
typedef void (*FreeFn)(EvalResult*);

EvalFn eval = nullptr;
FreeFn freemem = nullptr;

Napi::String EvalWrapped(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  std::string expr = info[0].As<Napi::String>();
  std::string vars = info[1].As<Napi::String>();

  std::cout << "Expression: " << expr << std::endl;
  std::cout << "Variables: " << vars << std::endl;

  EvalResult* result = eval(expr.c_str(), vars.c_str());
  if (!result) {
    Napi::TypeError::New(env, "Error evaluating expression").ThrowAsJavaScriptException();
    return Napi::String::New(env, "");
  }

  std::string strResult;
  if (result->ok) {
    std::cout << "Result: " << result->result << std::endl;
    strResult = std::to_string(result->result);
  } else {
    std::cerr << "Error: " << result->error << std::endl;
    strResult = result->error;
  }

  freemem(result);

  // #ifdef _WIN32
  // FreeLibrary(lib);
  // #else
  //   dlclose(lib);
  // #endif

  return Napi::String::New(env, strResult);
}

void LoadSharedLibrary() {
  #ifdef _WIN32
    freemem = (FreeFn)GetProcAddress(lib, "FreeEvalResult");
    eval = (EvalFn)GetProcAddress(lib, "EvalExpression");
  #else
    freemem = (FreeFn)dlsym(lib, "FreeEvalResult");
    eval = (EvalFn)dlsym(lib, "EvalExpression");
  #endif
}

void Load(const Napi::CallbackInfo& info) {
    if (lib != nullptr) return; // already loaded

    std::string path = info[0].As<Napi::String>();

    std::cout << "Loading library from: " << path << std::endl;
    #ifdef _WIN32
    lib = LoadLibraryA(path.c_str());
    #else
    std::cout << "prev dlopen " << std::endl;
    lib = dlopen(path.c_str(), RTLD_LAZY);
    std::cout << "post dlopen " << std::endl;
    #endif

    if (!lib) {
      std::cerr << "Failed to load library" << std::endl;
      return;
    }

    LoadSharedLibrary();
}

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // Obtain absolute path to the library

  exports.Set(Napi::String::New(env, "load"),
              Napi::Function::New(env, Load));

  exports.Set(Napi::String::New(env, "evalExpression"),
              Napi::Function::New(env, EvalWrapped));
  return exports;
}

NODE_API_MODULE(eval, Init)