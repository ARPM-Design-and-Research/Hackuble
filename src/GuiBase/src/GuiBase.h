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

#include <memory>



namespace SynGUI {

	class GuiBase : public EventHandler {
	public:
		GuiBase(int _frameWidth, int _frameHeight);

		int frameWidth, frameHeight;

		bool stopped = true;

		int maxWindow = 0;
		//GLFWwindow* window;
		std::vector<std::shared_ptr<BaseWindow>> windows;

		EventManager* eventManager;
		std::shared_ptr<Component> activeComponent = nullptr;

		TextLabel* fpsCounter;
		double lastTime = 0;
		int frameCount = 0;

		int startGui();
		void stopGui();
		std::shared_ptr<BaseWindow> addWindow(std::string title);
		void update();
		void render();

		void addEventToQueue(std::shared_ptr<Event> event);

		void OnMouseDown(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseUp(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseMove(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseWheel(std::shared_ptr<MouseEvent> eventArgs);
		void OnResize(std::shared_ptr<ResizeEvent> eventArgs);

		unsigned char* getPixelData();

		bool resized = false;
	};
}