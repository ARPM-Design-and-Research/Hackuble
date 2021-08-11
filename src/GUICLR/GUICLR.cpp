#pragma once

#include "GUICLR.h"

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



GUICLR::Component::Component(std::string title) {

}

GUICLR::Component::~Component() {

}

void GUICLR::Component::addSlider(std::string title, float currentValue, float startValue, float endValue) {

}


GUICLR::Slider::Slider() {

}

GUICLR::Slider::~Slider() {

}

GUICLR::BB::BB() {

}

GUICLR::BB::~BB() {

}