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

		Rectangle* addRectangle(glm::vec2 pos, glm::vec2 size, float r1, float r2, float r3, float r4, glm::vec3 col, float zDepth);
		void removeRectangle(Rectangle* rectangle);

		Bezier* addBezier(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col, int res);
		void removeBezier(Bezier* bezier);

		Icon* addIcon(const std::string& iconName, glm::vec2 pos, glm::vec2 size, Pivot pivot, float zDepth);
		void removeIcon(Icon* icon);

		TextLabel* addText(const std::string& _text, glm::vec2 _pos, glm::vec2 _size, float _fontSize = 50.0f, TextAlignment alignment = TextAlignment::LEFT, float _zDepth = 0.0f);
		void removeText(TextLabel* label);

		glm::vec2 screenToWorldSpace(glm::vec2 screenSpace);
	};
}