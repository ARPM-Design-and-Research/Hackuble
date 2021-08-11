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

LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);

class GuiContext {
private:
	std::thread glThread;
	HDC DC;
	HGLRC RC;
	HWND windowHandle;
	SynGUI::GuiBase* gui;

	bool initialized = false;
	bool onPaintEvent = false;

	static GuiContext* guiContext_;

	GuiContext();

public:

	GuiContext(GuiContext& other) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(const GuiContext&) = delete;
	/**
	 * This is the static method that controls the access to the singleton
	 * instance. On the first run, it creates a singleton object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */

	static GuiContext* GetInstance();

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

	void awaitInitialized();
	void awaitResize();

	int windowWidth;
	int windowHeight;

	SynGUI::BaseWindow* addWindow(std::string title);
};