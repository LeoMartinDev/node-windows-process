#ifndef NATIVE_EXTENSION_GRAB_W
#define NATIVE_EXTENSION_GRAB_W

#include "nan.h"
#include <windows.h>

class Window : public Nan::ObjectWrap {
public:
	static NAN_MODULE_INIT(Init);
	static NAN_METHOD(GetActiveWindow);
	static NAN_METHOD(GetWindowByClassName);
	static NAN_METHOD(GetWindowByTitleExact);
	static NAN_METHOD(EnumerateWindows);

	static Nan::Persistent<v8::Function> constructor;

private:
	explicit Window(HWND handle);
	~Window();

	static NAN_METHOD(New);

	static NAN_METHOD(exists);
	static NAN_METHOD(isVisible);
	static NAN_METHOD(getTitle);
	static NAN_METHOD(getHwnd);
	static NAN_METHOD(getClassName);
	static NAN_METHOD(getPid);

	static NAN_METHOD(getParent);
	static NAN_METHOD(getAncestor);
	static NAN_METHOD(getMonitor);

	static NAN_METHOD(setForegroundWindow);
	static NAN_METHOD(setWindowPos);
	static NAN_METHOD(showWindow);
	static NAN_METHOD(move);
	static NAN_METHOD(moveRelative);
	static NAN_METHOD(dimensions);

	HWND windowHandle;
};

#endif