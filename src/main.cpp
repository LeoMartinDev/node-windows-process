#include "nan.h"
#include <node.h>
#include <v8.h>
#include "nan.h"
#include "ProcessHelper.h";

namespace AukSwitcher {

    // NAN_METHOD is a Nan macro enabling convenient way of creating native node functions.
// It takes a method's name as a param. By C++ convention, I used the Capital cased name.
/*    NAN_METHOD(GetProcesses) {
*//*        std::vector<double> vec = { 5, 6, 7 };
        v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(vec.size());
        for (int i = 0; i < jsArr->Length(); i++) {
            double number = vec.at(i);
            v8::Local<v8::Value> jsElement = Nan::New(number);
            jsArr->Set(i, jsElement);
        }
        info.GetReturnValue().Set(jsArr);*//*
        std::vector<int> processes = Helpers::GetProcesses();
        v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(processes.size());

        for (int i = 0; i < jsArr->Length(); i++) {
            int number = processes.at(i);
            v8::Local<v8::Value> jsElement = Nan::New(number);
            jsArr->Set(i, jsElement);
        }
        info.GetReturnValue().Set(jsArr);
    };*/

#pragma unmanaged

    NAN_METHOD(GetProcessesByName) {
        std::vector<int> processes = Helpers::GetProcessesByName("Dofus");
        v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(processes.size());
        for (int i = 0; i < processes.size(); i++) {
            int number = processes.at(i);
            v8::Local<v8::Value> jsElement = Nan::New(number);
            jsArr->Set(i, jsElement);
        }
        info.GetReturnValue().Set(jsArr);
/*        int processes = GetAll();
        info.GetReturnValue().Set(processes);*/
    }

    NAN_METHOD(GetCurrentProcess) {
        v8::Local<v8::Object> process = Helpers::GetCurrentProcess();
/*        v8::Local <v8::Function> cons = Nan::New(Process::constructor)->GetFunction();

        v8::Local<v8::Object> jsSumVec = cons->NewInstance(0, nullptr);*/
        info.GetReturnValue().Set(process);
    }

// Module initialization logic
    NAN_MODULE_INIT(Initialize) {
        AukSwitcher::Process::Init(target);
        // Export the `Hello` function (equivalent to `export function Hello (...)` in JS)
        NAN_EXPORT(target, GetProcessesByName);
        NAN_EXPORT(target, GetCurrentProcess);
    };

// Create the module called "addon" and initialize it with `Initialize` function (created with NAN_MODULE_INIT macro)
    NODE_MODULE(addon, Initialize);
}
