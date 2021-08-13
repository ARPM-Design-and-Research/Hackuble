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

#include <memory>

LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);

class GuiContext {
private:
	std::thread glThread;
	HDC DC;
	HGLRC RC;
	HWND windowHandle;
	std::shared_ptr<SynGUI::GuiBase> gui;

	bool initialized = false;
	bool onPaintEvent = false;
	bool destroyEvent = false;

	static std::shared_ptr<GuiContext> guiContext_;

public:

	GuiContext();

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

	static std::shared_ptr<GuiContext> GetInstance();

	~GuiContext();
	void createContextThread(int windowWidth, int windowHeight);
	void createContext();

	unsigned char* getPixelData();
	void resize(int newWidth, int newHeight);

	void onMouseMove(float x, float y, int mouseButton);
	void onMouseDown(float x, float y, int mouseButton);
	void onMouseUp(float x, float y, int mouseButton);
	void onMouseWheel(float x, float y, int delta);

	void onPaint();

	void awaitInitialized();
	void awaitResize();

	void closeContext();

	int windowWidth;
	int windowHeight;

	std::shared_ptr<SynGUI::BaseWindow> addWindow(std::string title);
};