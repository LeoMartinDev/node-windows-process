#include "ProcessHelper.h"

namespace Helpers {
    v8::Local<v8::Object> GetCurrentProcess() {
        Diagnostics::Process ^currentProcess = Diagnostics::Process::GetCurrentProcess();
        v8::Local<v8::Function> cons = Nan::New(AukSwitcher::Process::constructor);
        const int argc = 1;

        v8::Local<v8::Value> argv[1] = {Nan::New((int)currentProcess->Handle.ToInt32())};
        return Nan::NewInstance(cons, argc, argv).ToLocalChecked();
    }

    std::vector<int> GetProcessesByName(const std::string& name) {
        String^ processName = gcnew String(name.c_str());
        array < Diagnostics::Process ^>^processesArray = Diagnostics::Process::GetProcessesByName(processName);
        std::vector<int> test = std::vector<int>();

        for (unsigned int i = 0; i <= sizeof(&processesArray) / sizeof(Diagnostics::Process^); i++)
        {
            System::Console::WriteLine(processesArray[i]->MainWindowTitle);
            test.push_back(processesArray[i]->Id);
        }
        System::Console::WriteLine(test.size());
        return test;
    }
}