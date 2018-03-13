//
// Created by leoma on 09/03/2018.
//

#ifndef NODEJSNATIVEADDON_GetProcessesByNameWorker_H
#define NODEJSNATIVEADDON_GetProcessesByNameWorker_H

#include "nan.h"
#include <nan.h>
#include <v8.h>
#include "Helpers.h"
#include "Process.h"

#pragma managed

class GetProcessesByNameWorker : public Nan::AsyncWorker {
public:
    GetProcessesByNameWorker(Nan::Callback *callback, const std::string& processName);
    ~GetProcessesByNameWorker();

    void Execute();
    void HandleOKCallback();

private:
    std::vector<Helpers::RawProcess*>* _processes;
    std::string _processName;
};

#endif //NODEJSNATIVEADDON_GetProcessesByNameWorker_H
