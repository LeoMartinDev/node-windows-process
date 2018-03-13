#include "Process.h"

Nan::Persistent<v8::Function> Process::constructor;

NAN_MODULE_INIT(Process::Init) {
    v8::Local <v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "setToForeground", setToForeground);
    Nan::SetPrototypeMethod(tpl, "terminate", terminate);

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("id").ToLocalChecked(), Process::HandleGetters);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("mainWindowTitle").ToLocalChecked(), Process::HandleGetters,
                     nullptr);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("mainWindowHandle").ToLocalChecked(), Process::HandleGetters,
                     nullptr);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
};

NAN_METHOD(Process::GetCurrent) {
    v8::Local <v8::Function> cons = Nan::New(constructor);
    Helpers::RawProcess* process = Helpers::GetCurrentProcess();
    const int argc = 4;
    v8::Local <v8::Value> argv[argc] =
    {
        Nan::New(process->handle),
        Nan::New(process->id),
        Nan::New(process->mainWindowTitle.c_str()).ToLocalChecked(),
        Nan::New(process->mainWindowHandle)
    };

    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
};

NAN_METHOD(Process::GetByName) {
    if (!info[0]->IsString()) {
        return Nan::ThrowError(Nan::New("Process::GetByName - name must be a string!").ToLocalChecked());
    }
    v8::Local <v8::Function> cons = Nan::New(constructor);
    v8::String::Utf8Value processName(info[0]);
    std::vector <Helpers::RawProcess*>* processes = Helpers::GetProcessesByName(std::string(*processName));
    v8::Local <v8::Array> jsProcessesArray = Nan::New<v8::Array>(processes->size());

    for (unsigned int i = 0; i < jsProcessesArray->Length(); i++) {
        const int argc = 4;
        v8::Local <v8::Value> argv[argc] = {
            Nan::New(processes->at(i)->handle),
            Nan::New(processes->at(i)->id),
            Nan::New(processes->at(i)->mainWindowTitle.c_str()).ToLocalChecked(),
            Nan::New(processes->at(i)->mainWindowHandle)
        };
        v8::Local<v8::Object> instance = Nan::NewInstance(cons, argc, argv).ToLocalChecked();

        jsProcessesArray->Set(i, instance);
    }
    info.GetReturnValue().Set(jsProcessesArray);
};

NAN_METHOD(Process::GetById) {
      if (!info[0]->IsNumber()) {
          return Nan::ThrowError(Nan::New("Process::GetById - id must be a number!").ToLocalChecked());
      }
      v8::Local <v8::Function> cons = Nan::New(constructor);
      int id = info[0]->NumberValue();
      try {
          Helpers::RawProcess* process = Helpers::GetProcessById(id);
          const int argc = 4;
          v8::Local <v8::Value> argv[argc] = {Nan::New(process->handle), Nan::New(process->id),
                                              Nan::New(process->mainWindowTitle.c_str()).ToLocalChecked(),
                                              Nan::New(process->mainWindowHandle)};

          info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
      }
      catch (const std::invalid_argument &e) {
          info.GetReturnValue().Set(Nan::Undefined());
      }
  };

  NAN_METHOD(Process::GetByIdAsync) {
      if (!info[0]->IsNumber()) {
          return Nan::ThrowError(Nan::New("Process::GetById - id must be a number!").ToLocalChecked());
      }
      if(!info[1]->IsFunction()) {
          return Nan::ThrowError(Nan::New("expected arg 1: function callback").ToLocalChecked());
      }

        Nan::AsyncQueueWorker(new GetProcessByIdWorker(
                new Nan::Callback(info[1].As<v8::Function>()),
                info[0]->NumberValue()
        ));
  };

NAN_METHOD(Process::TerminateProcessById) {
    if (!info[0]->IsNumber()) {
        return Nan::ThrowError(Nan::New("Process::GetById - id must be a number!").ToLocalChecked());
    }
    int id = info[0]->NumberValue();
    HANDLE handle = OpenProcess(PROCESS_TERMINATE, false, id);
    v8::Local <v8::Function> cons = Nan::New(constructor);

    if (handle != NULL) {
        TerminateProcess(handle, 0);
        CloseHandle(handle);
    }
    info.GetReturnValue().Set(Nan::Undefined());
};

NAN_METHOD(Process::GetCurrentAsync) {
    if(!info[0]->IsFunction()) {
        return Nan::ThrowError(Nan::New("expected arg 1: function callback").ToLocalChecked());
    }

    Nan::AsyncQueueWorker(new GetCurrentProcessWorker(
            new Nan::Callback(info[0].As<v8::Function>())
    ));
};

NAN_METHOD(Process::GetByNameAsync) {
    if (!info[0]->IsString()) {
        return Nan::ThrowError(Nan::New("expected arg 0: string").ToLocalChecked());
    }
    if(!info[1]->IsFunction()) {
        return Nan::ThrowError(Nan::New("expected arg 1: function callback").ToLocalChecked());
    }

    Nan::AsyncQueueWorker(new GetProcessesByNameWorker(
            new Nan::Callback(info[1].As<v8::Function>()),
            std::string(*v8::String::Utf8Value(info[0]))
    ));
};

NAN_GETTER(Process::HandleGetters) {
    Process *self = Nan::ObjectWrap::Unwrap<Process>(info.This());

    std::string propertyName = std::string(*Nan::Utf8String(property));
    if (propertyName == "id") {
        info.GetReturnValue().Set(self->id());
    } else if (propertyName == "mainWindowTitle") {
        info.GetReturnValue().Set(Nan::New(self->mainWindowTitle().c_str()).ToLocalChecked());
    } else if (propertyName == "mainWindowHandle") {
        info.GetReturnValue().Set(self->mainWindowHandle());
    } else {
        info.GetReturnValue().Set(Nan::Undefined());
    }
};

HANDLE Process::handle() const {
    return OpenProcess(PROCESS_ALL_ACCESS, false, _id);
}

int Process::id() const {
    return _id;
}

std::string Process::mainWindowTitle() const {
    return _mainWindowTitle;
}

int Process::mainWindowHandle() const {
    return _mainWindowHandle;
}

/*Nan::Persistent<v8::Function> &Process::constructor() {
    static Nan::Persistent<v8::Function> construct;
    return construct;
};*/

Process::Process(int handle, int id, std::string mainWindowTitle, int mainWindowHandle) : _handle(handle), _id(id),
                                                                                          _mainWindowTitle(
                                                                                                  mainWindowTitle),
                                                                                          _mainWindowHandle(
                                                                                                  mainWindowHandle) {}

Process::~Process() {};

NAN_METHOD(Process::New) {
    if (info.IsConstructCall()) {
        int handle = Nan::To<int>(info[0]).FromJust();
        int value = Nan::To<int>(info[1]).FromJust();
        v8::String::Utf8Value mainWindowTitle(info[2]->ToString());
        int mainWindowHandle = Nan::To<int>(info[3]).FromJust();

        Process *obj = new Process(handle, value, *mainWindowTitle, mainWindowHandle);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 4;

        v8::Local <v8::Value> argv[argc] = {info[0], info[1], info[2], info[3]};
        v8::Local <v8::Function> cons = Nan::New(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
};

NAN_METHOD(Process::setToForeground) {
    Process *obj = Nan::ObjectWrap::Unwrap<Process>(info.Holder());
/*     HWND hWnd = (HWND)obj->_mainWindowHandle;

    if (hWnd == NULL || !IsWindow(hWnd)) {
        info.GetReturnValue().Set(Nan::New(false));
    } else {
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        info.GetReturnValue().Set(Nan::New(true));
    } */
    HWND hWnd = (HWND) obj->mainWindowHandle();

    if (hWnd == NULL || !IsWindow(hWnd)) {
        info.GetReturnValue().Set(Nan::New(false));
    } else {
        SwitchToThisWindow(hWnd, true);
        info.GetReturnValue().Set(Nan::New(true));
    }
/*     HWND hWnd = (HWND) obj->mainWindowHandle();
    if(!IsWindow(hWnd)) info.GetReturnValue().Set(Nan::New(false));;

    BYTE keyState[256] = {0};
    //to unlock SetForegroundWindow we need to imitate Alt pressing
    if(GetKeyboardState((LPBYTE)&keyState))
    {
        if(!(keyState[VK_MENU] & 0x80))
        {
            keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        }
    }
    SetForegroundWindow(hWnd);
    if(GetKeyboardState((LPBYTE)&keyState))
    {
        if(!(keyState[VK_MENU] & 0x80))
        {
            keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
    }
    info.GetReturnValue().Set(Nan::New(true)); */
};

NAN_METHOD(Process::terminate) {
    Process *obj = Nan::ObjectWrap::Unwrap<Process>(info.Holder());
    HANDLE handle = obj->handle();
    
    if (handle != NULL) {
        TerminateProcess(handle, 0);
        CloseHandle(handle);
    }
    info.GetReturnValue().Set(Nan::Undefined());
};

NAN_MODULE_INIT(Init) {
    Process::Init(target);
    Nan::Set(target,
             Nan::New<v8::String>("getProcessById").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetById)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("getProcessByIdAsync").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetByIdAsync)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("getCurrentProcess").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetCurrent)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("getCurrentProcessAsync").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetCurrentAsync)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("terminateProcessById").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::TerminateProcessById)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("getProcessesByName").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetByName)).ToLocalChecked()
    );
    Nan::Set(target,
             Nan::New<v8::String>("getProcessesByNameAsync").ToLocalChecked(),
             Nan::GetFunction(
                     Nan::New<v8::FunctionTemplate>(Process::GetByNameAsync)).ToLocalChecked()
    );
}

NODE_MODULE(addon, Init)
