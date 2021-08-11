#pragma once

#include "GUICLR.h"

#include <string>

#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;

GUICLR::ManagedContext::ManagedContext() {
	GuiContext::GetInstance();
}

GUICLR::ManagedContext::~ManagedContext() {
	delete GuiContext::GetInstance();
}

void GUICLR::ManagedContext::getPixelData(array<unsigned char>^% pixelData) {
	unsigned char* data = GuiContext::GetInstance()->getPixelData();

	if (data != NULL) {
		Marshal::Copy(IntPtr(data), pixelData, 0, 4 * GuiContext::GetInstance()->windowWidth * GuiContext::GetInstance()->windowHeight);
	}
}

void  GUICLR::ManagedContext::createContext(int _windowWidth, int _windowHeight) {
	GuiContext::GetInstance()->createContextThread(_windowWidth, _windowHeight);
}

void GUICLR::ManagedContext::resize(int newWidth, int newHeight) {
	GuiContext::GetInstance()->resize(newWidth, newHeight);
}

void GUICLR::ManagedContext::onMouseMove(float x, float y, Int32 mouseButton) {
	GuiContext::GetInstance()->onMouseMove(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseDown(float x, float y, Int32 mouseButton) {
	GuiContext::GetInstance()->onMouseDown(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseUp(float x, float y, Int32 mouseButton) {
	GuiContext::GetInstance()->onMouseUp(x, y, mouseButton);
}

void GUICLR::ManagedContext::onMouseWheel(int delta) {
	GuiContext::GetInstance()->onMouseWheel(delta);
}

void GUICLR::ManagedContext::onPaint() {
	GuiContext::GetInstance()->onPaint();
}

void GUICLR::ManagedContext::awaitResize() {
	GuiContext::GetInstance()->awaitResize();
}

void GUICLR::ManagedContext::awaitInitialized() {
	GuiContext::GetInstance()->awaitInitialized();
}



GUICLR::Component::Component(System::String^ title) {

	baseComponent = GuiContext::GetInstance()->addWindow(marshal_as<std::string>(title));

	boundingBox.x0 = baseComponent->componentBoundingBox.x0;
	boundingBox.y0 = baseComponent->componentBoundingBox.y0;
	boundingBox.x1 = baseComponent->componentBoundingBox.x1;
	boundingBox.y1 = baseComponent->componentBoundingBox.y1;
}

GUICLR::Component::~Component() {

}

void GUICLR::Component::addSlider(System::String^ title, float currentValue, float startValue, float endValue) {
	baseComponent->addSlider(marshal_as<std::string>(title), currentValue, startValue, endValue);
}

void GUICLR::Component::setColor(float r, float g, float b) {
	baseComponent->setColor(glm::vec3(r/255.0f, g/255.0f, b/255.0f));
}

void GUICLR::Component::setTitle(System::String^ title) {
	baseComponent->setTitle(marshal_as<std::string>(title));
}

System::String^ GUICLR::Component::getTitle() {
	return marshal_as<System::String^>(baseComponent->getTitle());
}

/*GUICLR::BoundingBox GUICLR::Component::getBoundingBox() {
	return ;
}*/


GUICLR::Slider::Slider() {

}

GUICLR::Slider::~Slider() {

}

GUICLR::BoundingBox::BoundingBox() {

}

GUICLR::BoundingBox::~BoundingBox() {

}