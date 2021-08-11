#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <chrono>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/glcorearb.h"
#include "GL/wglext.h"


#include "Error.h"
#include "Shaders.h"
#include "Camera.h"
#include "RectangleRenderer.h"
#include "Rectangle.h"
#include "TextLabel.h"
#include "TextRenderer.h"
#include "RenderElement.h"
#include "Bezier.h"
#include "BezierRenderer.h"
#include "IconRenderer.h"
#include "GUIRenderer.h"
#include "Icon.h"
#include "Slider.h"

#include "BaseWindow.h"

#include "Event.h"

#include "gtc/random.hpp"

#include "dllExport.h"



namespace SynGUI {

	class GuiBase : public EventHandler {
	public:
		GuiBase(int _frameWidth, int _frameHeight);

		int frameWidth, frameHeight;

		bool stopped = true;

		int maxWindow = 0;
		//GLFWwindow* window;
		std::vector<BaseWindow*> windows;

		EventManager* eventManager;
		Component* activeWindow = nullptr;

		TextLabel* fpsCounter;
		double lastTime = 0;
		int frameCount = 0;

		int startGui();
		void stopGui();
		BaseWindow* addWindow(std::string title);
		void update();
		void render();

		void addEventToQueue(Event* event);

		void OnMouseDown(MouseEvent* eventArgs);
		void OnMouseUp(MouseEvent* eventArgs);
		void OnMouseMove(MouseEvent* eventArgs);
		void OnMouseWheel(MouseEvent* eventArgs);
		void OnResize(ResizeEvent* eventArgs);

		unsigned char* getPixelData();

		bool resized = false;
	};
}