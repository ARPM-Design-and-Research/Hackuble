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

GuiBase* createClass(int frameWidth, int frameHeight) {
	return new GuiBase(frameWidth, frameHeight);
}


void destroyClass(GuiBase* ptr) {

	if (ptr != nullptr) {

		if (!ptr->stopped) {
			ptr->stopGui();
		}

		delete ptr;
	}
}


void startGui(GuiBase* ptr) {
	ptr->startGui();
	ptr->stopped = false;
}


void stopGui(GuiBase* ptr) {
	ptr->stopGui();
	ptr->stopped = true;
}


void addWindow(GuiBase* ptr) {
	ptr->addWindow();
}


void update(GuiBase* ptr) {
	ptr->update();
}


void render(GuiBase* ptr) {
	ptr->render();
}

GuiBase::GuiBase(int _frameWidth, int _frameHeight) {
	frameWidth = _frameWidth;
	frameHeight = _frameHeight;

	startGui();
}

void GuiBase::OnMouseDown(MouseEvent* eventArgs) {

	if (activeWindow == nullptr && eventArgs->button == MouseButton::LEFT) {
		for (BaseWindow* window : windows) {
			BoundingBox b = ((Component*)window)->componentBoundingBox;

			glm::vec2 mousePos = eventArgs->pos;
			
			if (mousePos.x > b.x0 && mousePos.x < b.x1 && mousePos.y > b.y0 && mousePos.y < b.y1) {
				eventManager->addListener(window);
				activeWindow = window;
				break;
			}
		}
	}
}

void GuiBase::OnMouseUp(MouseEvent* eventArgs) {

	if (activeWindow != nullptr && eventArgs->button == MouseButton::LEFT) {
		eventManager->removeListener((BaseWindow*)activeWindow);
		activeWindow = nullptr;
	}
}

void GuiBase::OnMouseMove(MouseEvent* eventArgs) {

}

void GuiBase::OnMouseWheel(MouseEvent* eventArgs) {
	
}

void GuiBase::addWindow() {

	maxWindow++;
	//TODO: Fix Memory leaks for BaseWindow
	BaseWindow* windowComp = new BaseWindow("Window " + std::to_string(maxWindow + 1), "add.png", glm::vec2(120.0f, 70.0f), glm::vec2(glm::linearRand(-150.0f,150.0f),glm::linearRand(-150.0f,150.0f)), glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f)), 0.001f * (float)(maxWindow + 1));
	windows.push_back(windowComp);

	//eventManager->addListener((EventHandler*)windowComp);

	int numSlider = glm::linearRand(1, 7);

	for (int j = 0; j < numSlider; j++) {
		Slider* slider = windowComp->addSlider("Slider " + std::to_string(j), glm::linearRand(0.0f, 115.0f), 0.0f, 115.0f);
	}
}

void GuiBase::stopGui() {

	for (int i = 0; i < windows.size(); i++) {
		delete windows.at(i);
	}

	windows.clear();
	maxWindow = 0;

	//Cleanup
	GUIRenderer::GetInstance()->deinit();

	delete eventManager;
}

void GuiBase::update() {
	eventManager->pollEvents();
	GUIRenderer::GetInstance()->update();
}

void GuiBase::render() {
	GUIRenderer::GetInstance()->render();
}

unsigned char* GuiBase::getPixelData() {
	return GUIRenderer::GetInstance()->getPixelData();
}

void GuiBase::resize(int newWidth, int newHeight) {
	GUIRenderer::GetInstance()->setScreenSize(newWidth, newHeight);
}

void GuiBase::addEventToQueue(Event* _event) {
	eventManager->addEvent(_event);
}

int GuiBase::startGui()
{
	GUIRenderer::GetInstance()->init(frameWidth, frameHeight);

	//GUIRenderer::GetInstance()->setScreenSize(frameWidth, frameHeight);
	Camera::GetInstance()->setPosition(glm::vec3(-frameWidth, -frameHeight, 0.0f));
	Camera::GetInstance()->setZoom(glm::vec3(3.0f));

	int numWindow = 0;

	IconRenderer::GetInstance()->addIcon("triangle.png");
	IconRenderer::GetInstance()->addIcon("add.png", glm::vec2(15.0f, 0.0f));
	IconRenderer::GetInstance()->addIcon("perlin.png", glm::vec2(30.0f, 0.0f));
	IconRenderer::GetInstance()->addIcon("text.png", glm::vec2(45.0f, 0.0f));

	int frame = 0;

	eventManager = new EventManager();

	eventManager->addListener(this);
	eventManager->addListener(Camera::GetInstance());

	addWindow();
	addWindow();
	addWindow();

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
