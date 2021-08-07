#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <thread>

#include <string>
#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/wglext.h"

#include "GuiBase.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

class GuiContext {
private:
	std::thread glThread;
	HDC DC;
	HGLRC RC;
	HWND windowHandle;
	SynGUI::GuiBase* gui;

	bool initialized = false;
	bool onPaintEvent = false;

public:
	GuiContext();
	~GuiContext();
	void createContextThread(int windowWidth, int windowHeight);
	int createContext();

	unsigned char* getPixelData();
	void resize(int newWidth, int newHeight);
	void onMouseMove(float x, float y, int mouseButton);
	void onMouseDown(float x, float y, int mouseButton);
	void onMouseUp(float x, float y, int mouseButton);
	void onMouseWheel(int delta);

	void onPaint();

	bool isInitialized();

	int windowWidth;
	int windowHeight;
};


namespace GUICLR {
	public ref class ManagedContext
	{
	private:
		GuiContext* mc;
	public:
		ManagedContext();
		~ManagedContext();

		void createContext(int windowWidth, int windowHeight);
		void resize(int newWidth, int newHeight);
		void getPixelData(array<unsigned char>^% pixelData);

		void onMouseMove(float x, float y, Int32 mouseButton);
		void onMouseDown(float x, float y, Int32 mouseButton);
		void onMouseUp(float x, float y, Int32 mouseButton);
		void onMouseWheel(int delta);

		void onPaint();

		bool isInitialized();
		// TODO: Add your methods for this class here.
	};
}

[STAThreadAttribute]

LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);


std::string name = "Syncode GUI";

/* quit flag is here to prevent
   quitting when recreating the window (see the window_procedure function) */

bool quit = false;

GUICLR::ManagedContext::ManagedContext() {
	mc = new GuiContext();
}

GUICLR::ManagedContext::~ManagedContext() {
	delete mc;
}

void GUICLR::ManagedContext::getPixelData(array<unsigned char>^% pixelData) {
	unsigned char* data = mc->getPixelData();
	
	if (data != NULL) {
		Marshal::Copy(IntPtr(data), pixelData, 0, 4*mc->windowWidth*mc->windowHeight);
	}
}

void  GUICLR::ManagedContext::createContext(int _windowWidth, int _windowHeight) {
  mc->createContextThread(_windowWidth, _windowHeight);
}

void GUICLR::ManagedContext::resize(int newWidth, int newHeight) {
	mc->resize(newWidth, newHeight);
}

void GUICLR::ManagedContext::onMouseMove(float x, float y, Int32 mouseButton) {
	mc->onMouseMove(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseDown(float x, float y, Int32 mouseButton) {
	mc->onMouseDown(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseUp(float x, float y, Int32 mouseButton) {
	mc->onMouseUp(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseWheel(int delta) {
	mc->onMouseWheel(delta);
}

void GUICLR::ManagedContext::onPaint() {
	mc->onPaint();
}

bool GUICLR::ManagedContext::isInitialized() {
	return mc->isInitialized();
}

GuiContext::GuiContext() {
	
}

GuiContext::~GuiContext() {

}

bool GuiContext::isInitialized() {
	return initialized;
}

unsigned char* GuiContext::getPixelData() {
	return gui->getPixelData();
}

void GuiContext::createContextThread(int _windowWidth, int _windowHeight) {
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	glThread = std::thread(&GuiContext::createContext, this);
	glThread.detach();
}

void GuiContext::resize(int newWidth, int newHeight) {

	if (isInitialized()) {
		windowWidth = newWidth;
		windowHeight = newHeight;

		wglMakeCurrent(DC, RC);

		DWORD style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		RECT r = RECT();
		r.top = 0;
		r.bottom = newWidth;
		r.left = 0;
		r.right = newHeight;

		AdjustWindowRect(&r, style, FALSE);

		SetWindowPos(windowHandle, NULL, 0, 0, r.right - r.left, r.bottom - r.top, NULL);

		gui->resize(newWidth, newHeight);
	}
}


static MouseButton convertMouseButton(int button) {
	MouseButton buttonClicked;

	switch (button) {
	case 0:
		buttonClicked = MouseButton::NONE;
		break;
	case 1048576:
		buttonClicked = MouseButton::LEFT;
		break;
	case 2097152:
		buttonClicked = MouseButton::RIGHT;
		break;
	case 4194304:
		buttonClicked = MouseButton::MIDDLE;
		break;
	}

	return buttonClicked;
}

void GuiContext::onMouseMove(float x, float y, int mouseButton) {

	if (initialized)
		gui->addEventToQueue(new SynGUI::MouseEvent(EventType::MOUSEMOVE, x, y, convertMouseButton(mouseButton)));
}

void GuiContext::onMouseDown(float x, float y, int mouseButton) {

	if (initialized)
		gui->addEventToQueue(new SynGUI::MouseEvent(EventType::MOUSEDOWN, x, y, convertMouseButton(mouseButton)));
}

void GuiContext::onMouseUp(float x, float y, int mouseButton) {

	if (initialized)
		gui->addEventToQueue(new SynGUI::MouseEvent(EventType::MOUSEUP, x, y, convertMouseButton(mouseButton)));
}

void GuiContext::onMouseWheel(int delta) {

	if (initialized)
		gui->addEventToQueue(new SynGUI::MouseEvent(EventType::MOUSEWHEEL,delta));
}

void GuiContext::onPaint() {

	onPaintEvent = true;
}

int GuiContext::createContext() {

	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = window_procedure;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = L"Core";

	LPTSTR windowClass = MAKEINTATOM(RegisterClassEx(&wcex));

	if (windowClass == 0) {
		OutputDebugString(L"registerClass() failed.");
		return 1;
	}

	// create temporary window

	HWND fakeWND = CreateWindow(
		windowClass, L"Fake Window",
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,						// position x, y
		1, 1,						// width, height
		NULL, NULL,					// parent window, menu
		hInstance, NULL);			// instance, param

	HDC fakeDC = GetDC(fakeWND);	// Device Context

	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;

	const int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
	if (fakePFDID == 0) {
		OutputDebugString(L"ChoosePixelFormat() failed.");
		return 1;
	}

	if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
		OutputDebugString(L"SetPixelFormat() failed.");
		return 1;
	}

	HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex

	if (fakeRC == 0) {
		OutputDebugString(L"wglCreateContext() failed.");
		return 1;
	}

	if (wglMakeCurrent(fakeDC, fakeRC) == false) {
		OutputDebugString(L"wglMakeCurrent() failed.");
		return 1;
	}

	// get pointers to functions (or init opengl loader here)

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		OutputDebugString(L"glewInit() failed.");
	}


	// create a new window and context
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT r = RECT();
	r.top = 0;
	r.bottom = windowHeight;
	r.left = 0;
	r.right = windowWidth;

	AdjustWindowRect(&r, style, FALSE);

	HWND WND = CreateWindow(
		windowClass, std::wstring(name.begin(), name.end()).c_str(),	// class name, window name
		style,							// styles
		0, 0,							// posx, posy. If x is set to CW_USEDEFAULT y is ignored
		r.right-r.left, r.bottom-r.top,	// width, height
		NULL, NULL,						// parent window, menu
		hInstance, NULL);				// instance, param*/

	DC = GetDC(WND);

	windowHandle = WND;

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		0
	};

	int pixelFormatID; UINT numFormats;
	const bool status = wglChoosePixelFormatARB(DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) {
		OutputDebugString(L"wglChoosePixelFormatARB() failed.");
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(DC, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 3;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
				0
	};


	RC = wglCreateContextAttribsARB(DC, 0, contextAttribs);
	if (RC == NULL) {
		OutputDebugString(L"wglCreateContextAttribsARB() failed.");
		return 1;
	}

	// delete temporary context and window

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(fakeWND, fakeDC);
	DestroyWindow(fakeWND);
	if (!wglMakeCurrent(DC, RC)) {
		OutputDebugString(L"wglMakeCurrent() failed.");
		return 1;
	}

	// init opengl loader here (extra safe version)

	err = glewInit();

	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		OutputDebugString(L"glewInit() failed.");
	}

	std::string openglVersion = (const char*)glGetString(GL_VERSION);
	std::string messageString = "Loaded: " + openglVersion + "\n";
	std::wstring str = std::wstring(messageString.begin(), messageString.end());
	LPCWSTR out = str.c_str();
	OutputDebugString(out);

	//SetWindowText(WND, reinterpret_cast<LPCSTR>(glGetString(GL_VERSION)));
	//ShowWindow(WND, SW_SHOW);


	MSG msg;

	gui = new SynGUI::GuiBase(windowWidth, windowHeight);

	initialized = true;

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (onPaintEvent) {
			gui->update();
			gui->render();

			SwapBuffers(DC);

			onPaintEvent = false;
		}
	}
	
	initialized = false;

	gui->stopGui();
	delete gui;
	/* ********** */

	return 0;
}

// Procedure that processes window events
LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM param_w, LPARAM param_l)
{
	/* When destroying the dummy window, WM_DESTROY message is going to be sent,
	   but we don't want to quit the application then, and that is controlled by
	   the quit flag. */

	switch (message) {
	case WM_DESTROY:
		if (!quit) quit = true;
		else PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(window_handle, message, param_w, param_l);
}

