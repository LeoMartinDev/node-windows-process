#include "GetCurrentProcessWorker.h"

GetCurrentProcessWorker::GetCurrentProcessWorker(Nan::Callback *callback)
        : AsyncWorker(callback) {};

GetCurrentProcessWorker::~GetCurrentProcessWorker() {};

void GetCurrentProcessWorker::Execute() {
    this->_process = Helpers::GetCurrentProcess();
};

void GetCurrentProcessWorker::HandleOKCallback() {
    v8::Local <v8::Function> cons = Nan::New(Process::constructor);
    v8::Local <v8::Value> consArgv[4] =
    {
         Nan::New(this->_process->handle),
         Nan::New(this->_process->id),
         Nan::New(this->_process->mainWindowTitle.c_str()).ToLocalChecked(),
         Nan::New(this->_process->mainWindowHandle)
     };
    v8::Local<v8::Object> instance = Nan::NewInstance(cons, 4, consArgv).ToLocalChecked();
    v8::Local <v8::Value> argv[2] =
    {
        Nan::Null(),
        instance,
    };
    this->callback->Call(2, argv);
};