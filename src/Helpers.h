//
// Created by leoma on 09/03/2018.
//

#ifndef NODEJSNATIVEADDON_HELPERS_H
#define NODEJSNATIVEADDON_HELPERS_H

#include "nan.h"
#include "nan.h"
#include <v8.h>
#include <map>
#include <exception>
#include <msclr\marshal_cppstd.h>
#include <windows.h>
#include <stdexcept>

#pragma unmanaged

#using <mscorlib.dll>
#using <System.dll>

using namespace System;

#pragma managed

namespace Helpers {

    struct RawProcess {
        int handle;
        int id;
        std::string mainWindowTitle;
        int mainWindowHandle;
    };

    std::vector<RawProcess*>* GetProcessesByName(const std::string &name);

    RawProcess* GetCurrentProcess();

    RawProcess* GetProcessById(const int id);

}

#endif //NODEJSNATIVEADDON_HELPERS_H
