//
// Created by leoma on 09/03/2018.
//

#ifndef NODEJSNATIVEADDON_GetProcessByIdWorker_H
#define NODEJSNATIVEADDON_GetProcessByIdWorker_H

#include "nan.h"
#include <nan.h>
#include <v8.h>
#include "Helpers.h"
#include "Process.h"

#pragma managed;

class GetProcessByIdWorker : public Nan::AsyncWorker {
public:
    GetProcessByIdWorker(Nan::Callback *callback, int processId);
    ~GetProcessByIdWorker();

    void Execute();
    void HandleOKCallback();

private:
    Helpers::RawProcess* _process;
    int _processId;
};

#endif //NODEJSNATIVEADDON_GetProcessByIdWorker_H
