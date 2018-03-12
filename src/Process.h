#ifndef NODEJSNATIVEADDON_PROCESS_H
#define NODEJSNATIVEADDON_PROCESS_H

#include "nan.h"
#include "nan.h"
#include <v8.h>
#include <map>
#include <exception>
#include <windows.h>
#include <Winuser.h>
#include <Winbase.h>
#include <stdexcept>
#include "GetCurrentProcessWorker.h"
#include "GetProcessesByNameWorker.h"
#include "GetProcessByIdWorker.h"
#include "Helpers.h"

#pragma unmanaged

class Process : public Nan::ObjectWrap {

public:
    static NAN_MODULE_INIT(Init);

    static NAN_METHOD(GetCurrent);
    static NAN_METHOD(GetCurrentAsync);
    static NAN_METHOD(GetByName);
    static NAN_METHOD(GetByNameAsync);
    static NAN_METHOD(GetById);
    static NAN_METHOD(GetByIdAsync);
    static NAN_METHOD(TerminateProcessById);

    static NAN_GETTER(HandleGetters);

    inline HANDLE handle() const;
    inline int id() const;
    inline std::string mainWindowTitle() const;
    inline int mainWindowHandle() const;
    //static inline Nan::Persistent<v8::Function> &constructor();
    static Nan::Persistent<v8::Function> constructor;

private:
    explicit Process(int handle, int id, std::string mainWindowTitle, int mainWindowHandle);
    ~Process();

    static NAN_METHOD(New);
    static NAN_METHOD(setToForeground);
    static NAN_METHOD(terminate);

    int _handle;
    int _id;
    std::string _mainWindowTitle;
    int _mainWindowHandle;
};

#endif //NODEJSNATIVEADDON_PROCESS_H
