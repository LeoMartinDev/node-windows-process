#include "SetToForegroundWorker.h"

SetToForegroundWorker::SetToForegroundWorker(Nan::Callback *callback, HWND windowHandle)
        : AsyncWorker(callback), _windowHandle(windowHandle) {};

SetToForegroundWorker::~SetToForegroundWorker() {};

void SetToForegroundWorker::Execute() {
    HWND hWnd = this->_windowHandle;

    if (!IsWindow(hWnd))
        this->_result = false;

    BYTE keyState[256] = {0};
    //to unlock SetForegroundWindow we need to imitate Alt pressing
    if (GetKeyboardState((LPBYTE)&keyState))
    {
        if (!(keyState[VK_MENU] & 0x80))
        {
            keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        }
    }
    SetForegroundWindow(hWnd);
    if (GetKeyboardState((LPBYTE)&keyState))
    {
        if (!(keyState[VK_MENU] & 0x80))
        {
            keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
    }
    this->_result = true;
};

void SetToForegroundWorker::HandleOKCallback() {
    v8::Local <v8::Value> argv[2] =
    {
        Nan::Null(),
        Nan::New(this->_result),
    };
    this->callback->Call(2, argv);
};