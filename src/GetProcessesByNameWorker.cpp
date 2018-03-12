#include "GetProcessesByNameWorker.h"

GetProcessesByNameWorker::GetProcessesByNameWorker(Nan::Callback *callback, const std::string &processName)
        : AsyncWorker(callback), _processName(processName) {};

GetProcessesByNameWorker::~GetProcessesByNameWorker() {};

void GetProcessesByNameWorker::Execute() {
    this->_processes = Helpers::GetProcessesByName(this->_processName);
};

void GetProcessesByNameWorker::HandleOKCallback() {
    v8::Local <v8::Function> cons = Nan::New(Process::constructor);
    v8::Local <v8::Array> jsProcessesArray = Nan::New<v8::Array>(this->_processes->size());

    for (unsigned int i = 0; i < jsProcessesArray->Length(); i++) {
        const int consArgc = 4;
        v8::Local <v8::Value> consArgv[consArgc] = {
            Nan::New(this->_processes->at(i)->handle),
            Nan::New(this->_processes->at(i)->id),
            Nan::New(this->_processes->at(i)->mainWindowTitle.c_str()).ToLocalChecked(),
            Nan::New(this->_processes->at(i)->mainWindowHandle)
        };
        v8::Local<v8::Object> instance = Nan::NewInstance(cons, consArgc, consArgv).ToLocalChecked();
        jsProcessesArray->Set(i, instance);
    }
    v8::Local <v8::Value> argv[2] =
        {
            Nan::Null(),
            jsProcessesArray,
        };
    this->callback->Call(2, argv);
};