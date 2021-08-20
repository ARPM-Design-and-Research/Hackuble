#pragma once

#include "GuiBase.h"

/*The current example demonstrates how various functions can be used to create a gui interface.
* 
* The setup code for this application is using GLFW, for windows form setup and OpenGL context
* management.
* 
* To get the rendering engine running, setup the OpenGL contex and then call:
* GUIRenderer::GetInstance()->init(frameWidth, frameHeight);
* 
* **Note that the main rendering engine is a singleton, and you have to call the GetInstance()
* function to get a pointer to the current rendering instance.
* 
* The camera is also a singleton. To setup the camera position call
* Camera::GetInstance()->setPosition(glm::vec3(x,y,z));
* 
* **Note that this engine uses the glm math library
* 
* In the main application loop, call the update and render function on the GUIRenderer
* GUIRenderer::GetInstance()->update();
* GUIRenderer::GetInstance()->render();
* 
* Upon closing the application, make sure to deinit the renderer:
* GUIRenderer::GetInstance()->deinit();
* 
* 
* To make a new window for example, simply reference the BaseWindow class:
* BaseWindow* window = new BaseWindow("Title");
* The pointer to the window allows for modifiying and adding more components to the window:
* Slider* slider = window->addSlider("Slider 1");
* 
* There function return a pointer to the created component. They can then be used to modify the component as required.
* All the rendering and updating happens automatically by the GUIRenderer
*/

//TODO: create shader class to efficiently handle uniforms



/*DLL_CPP_CLASS
class GuiBase;

extern "C" __declspec(dllexport)
GuiBase * createClass(int frameWidth, int frameHeight);

extern "C" __declspec(dllexport)
void destroyClass(GuiBase * ptr);

extern "C" __declspec(dllexport)
void startGui(GuiBase * ptr);

extern "C" __declspec(dllexport)
void stopGui(GuiBase * ptr);

extern "C" __declspec(dllexport)
void update(GuiBase * ptr);

extern "C" __declspec(dllexport)
void render(GuiBase * ptr);*/

//Export add function from dll
/*DLL_C_FUNCTION
int add(int a, int b) {
	return a + b;
}*/

using namespace SynGUI;

GuiBase::GuiBase(int _frameWidth, int _frameHeight) {
	frameWidth = _frameWidth;
	frameHeight = _frameHeight;

	startGui();
}

void GuiBase::OnMouseDown(std::shared_ptr<MouseEvent> eventArgs) {

	if (activeComponent == nullptr && eventArgs->button == MouseButton::LEFT) {
		for (std::shared_ptr<BaseWindow> window : windows) {
			BoundingBox b = std::static_pointer_cast<Component>(window)->componentBoundingBox;

			glm::vec2 mousePos = eventArgs->pos;
			
			if (window->childComponents.size() > 0) {
				for (std::shared_ptr<Component> child : window->childComponents) {
					if (mousePos.x > child->componentBoundingBox.x0 && mousePos.x < child->componentBoundingBox.x1 && mousePos.y > child->componentBoundingBox.y0 && mousePos.y < child->componentBoundingBox.y1) {
						eventManager->addListener(child);
						activeComponent = child;
						break;
					}
				}
			}

			if (activeComponent == nullptr && mousePos.x > b.x0 && mousePos.x < b.x1 && mousePos.y > b.y0 && mousePos.y < b.y1) {
				eventManager->addListener(window);
				activeComponent = window;
				break;
			}
		}
	}
}

void GuiBase::OnMouseUp(std::shared_ptr<MouseEvent> eventArgs) {

	if (activeComponent != nullptr && eventArgs->button == MouseButton::LEFT) {
		eventManager->removeListener(activeComponent);
		activeComponent->OnMouseUp(eventArgs);
		activeComponent = nullptr;
	}
}

void GuiBase::OnMouseMove(std::shared_ptr<MouseEvent> eventArgs) {

}

void GuiBase::OnMouseWheel(std::shared_ptr<MouseEvent> eventArgs) {
	
}

std::shared_ptr<BaseWindow> GuiBase::addWindow(std::string title) {

	maxWindow++;
	std::shared_ptr<BaseWindow> windowComp = std::make_shared<BaseWindow>(title, "add.png", glm::vec2(120.0f, 70.0f), glm::vec2(0,0), glm::vec3(1.0f), 0.001f * (float)(maxWindow + 1));
	windows.push_back(windowComp);

	//eventManager->addListener((EventHandler*)windowComp);

	/*int numSlider = glm::linearRand(1, 7);

	for (int j = 0; j < numSlider; j++) {
		Slider* slider = windowComp->addSlider("Slider " + std::to_string(j), glm::linearRand(0.0f, 115.0f), 0.0f, 115.0f);
	}*/

	return windowComp;
}

SynGUI::Rectangle* GuiBase::addRectangle(glm::vec2 pos, glm::vec2 size, float r1, float r2, float r3, float r4, glm::vec3 col, Pivot pivot, float zDepth) {
	return RectangleRenderer::GetInstance()->addRectangle(pos, size, r1, r2, r3, r4, col, pivot, zDepth);
}

void GuiBase::removeRectangle(Rectangle* rectangle) {
	RectangleRenderer::GetInstance()->removeRectangle(rectangle);
}

SynGUI::Bezier * GuiBase::addBezier(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col, int res) {
	return BezierRenderer::GetInstance()->addBezierCurve(v1, v2, v3, v4, col, res);
}

void GuiBase::removeBezier(Bezier* bezier) {
	BezierRenderer::GetInstance()->removeBezierCurve(bezier);
}

SynGUI::Icon* GuiBase::addIcon(const std::string& iconName, glm::vec2 pos, glm::vec2 size, Pivot pivot, float zDepth) {
	return IconRenderer::GetInstance()->addIcon(iconName, pos, size, pivot, zDepth);
}

void GuiBase::removeIcon(Icon* icon) {
	IconRenderer::GetInstance()->removeIcon(icon);
}

SynGUI::TextLabel* GuiBase::addText(const std::string& _text, glm::vec2 _pos, glm::vec2 _size, float _fontSize, TextAlignment alignment, Pivot pivot, float _zDepth) {
	return TextRenderer::GetInstance()->addText(_text, _pos, _size, _fontSize, alignment, pivot,_zDepth);
}

void GuiBase::removeText(TextLabel* label) {
	TextRenderer::GetInstance()->removeText(label);
}

void GuiBase::enableTextBoundingBoxDisplay(bool visible) {
	TextRenderer::GetInstance()->enableBoundingBox(visible);
}

void GuiBase::enableTextGlyphBoxDisplay(bool visible) {
	TextRenderer::GetInstance()->enableGlyphBox(visible);
}

glm::vec2 GuiBase::screenToWorldSpace(glm::vec2 screenSpace) {
	glm::vec4 worldSpace = glm::inverse(Camera::GetInstance()->getViewMatrix())* glm::vec4(screenSpace.x * 2.0f, screenSpace.y * 2.0f, 0.0f, 1.0f);
	return glm::vec2(worldSpace.x, worldSpace.y);
}

void GuiBase::stopGui() {

	/*for (int i = 0; i < windows.size(); i++) {
		delete windows.at(i);
	}*/

	windows.clear();
	maxWindow = 0;

	//Cleanup
	GUIRenderer::GetInstance()->deinit();

	delete eventManager;
}

void GuiBase::update() {
	GUIRenderer::GetInstance()->update();

	eventManager->pollEvents();
}

using namespace std::chrono;

void GuiBase::render() {

	GUIRenderer::GetInstance()->render();

	frameCount++;
	double t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	double delta = t - lastTime;

	lastTime = t;

	fpsCounter->updateText(std::to_string(1000/delta));
	//fpsCounter->updateText(std::to_string(Camera::GetInstance()->zoom.x));

}

unsigned char* GuiBase::getPixelData() {
	return GUIRenderer::GetInstance()->getPixelData();
}

void GuiBase::OnResize(std::shared_ptr<ResizeEvent> eventArgs) {
	GUIRenderer::GetInstance()->setScreenSize(eventArgs->width, eventArgs->height);
	frameWidth = eventArgs->width;
	frameHeight = eventArgs->height;
	resized = true;
}

void GuiBase::addEventToQueue(std::shared_ptr<Event> _event) {
	eventManager->addEvent(_event);
}

int GuiBase::startGui()
{
	GUIRenderer::GetInstance()->init(frameWidth, frameHeight);

	//GUIRenderer::GetInstance()->setScreenSize(frameWidth, frameHeight);
	Camera::GetInstance()->setPosition(glm::vec3(-frameWidth, -frameHeight, 0.0f));

	int numWindow = 0;

	/*IconRenderer::GetInstance()->addIcon("triangle.png");
	IconRenderer::GetInstance()->addIcon("add.png", glm::vec2(15.0f, 0.0f));
	IconRenderer::GetInstance()->addIcon("perlin.png", glm::vec2(30.0f, 0.0f));
	IconRenderer::GetInstance()->addIcon("text.png", glm::vec2(45.0f, 0.0f));*/
	//BezierRenderer::GetInstance()->addBezierCurve(glm::vec2(0.0f), glm::vec2(20.0f, 20.0f), glm::vec2(80.0f, 20.0f), glm::vec2(100.0f, 0.0f));
	
	fpsCounter = TextRenderer::GetInstance()->addText("1000.0000", glm::vec2(200.0f,50.0f), glm::vec2(100.0f,50.0f),25.0f, TextAlignment::LEFT);

	int frame = 0;

	eventManager = new EventManager();

	//eventManager->addListener(shared_from_this());
	eventManager->addListener(std::static_pointer_cast<EventHandler>(Camera::GetInstance()));

	//addWindow("Component 1");
	//addWindow("Component 2");
	//addWindow("Component 3");

	/*while (!glfwWindowShouldClose(window))
	{

		glfwWaitEvents();
		//glfwPollEvents();

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//Convert screen space to world space coordinates
		glm::vec4 mousePos = glm::inverse(Camera::GetInstance()->getViewMatrix()) * glm::vec4(xpos * 2.0f, ypos * 2.0f, 0.0f, 1.0f);


		if (windows.size() > 0) {
			//Switch between active window by Z and X keys
			int state = glfwGetKey(window, GLFW_KEY_Z);
			if (state == GLFW_PRESS) {
				numWindow--;

				if (numWindow == -1)
					numWindow = maxWindow - 1;
			}

			state = glfwGetKey(window, GLFW_KEY_X);
			if (state == GLFW_PRESS) {
				numWindow++;

				if (numWindow == maxWindow)
					numWindow = 0;
			}

			state = glfwGetKey(window, GLFW_KEY_C);
			if (state == GLFW_PRESS) {
				addWindow();
			}

			//Update position of selected window
			windows.at(numWindow)->setPosition(glm::vec2(mousePos.x, mousePos.y));
		}

		GUIRenderer::GetInstance()->update();
		GUIRenderer::GetInstance()->render();

		//For profiling
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time_span = t2 - t1;
		std::cout << time_span.count() << " ms " << 1000.0f/time_span.count() << " fps " << std::endl;

		//Swap forward and backward buffer
		glfwSwapBuffers(window);

		frame++;
	}*/

	/*for (int i = 0; i < windows.size(); i++) {
		delete windows.at(i);
	}

	windows.clear();
	maxWindow = 0;

	//Cleanup

	GUIRenderer::GetInstance()->deinit();
	//Cleanup
	//glfwDestroyWindow(window);
	//glfwTerminate();

	_CrtDumpMemoryLeaks();*/

	return 0;
}

