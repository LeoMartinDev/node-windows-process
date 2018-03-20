//
// Created by leoma on 09/03/2018.
//

#ifndef NODEJSNATIVEADDON_SETTOFOREGROUNDWORKER_H
#define NODEJSNATIVEADDON_SETTOFOREGROUNDWORKER_H

#include "nan.h"
#include <nan.h>
#include <v8.h>
#include "Helpers.h"
#include "Process.h"

#pragma managed

class SetToForegroundWorker : public Nan::AsyncWorker {
public:
    SetToForegroundWorker(Nan::Callback *callback, HWND windowHandle);
    ~SetToForegroundWorker();

    void Execute();
    void HandleOKCallback();

private:
    HWND _windowHandle;
    BOOLEAN _result = false;
};

#endif //NODEJSNATIVEADDON_SETTOFOREGROUNDWORKER_H
