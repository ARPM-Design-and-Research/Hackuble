#pragma once

#include "Component_CLR.h"
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace GUICLR;
using namespace msclr::interop;

GUICLR::Component::Component(System::String^ title) {

	//TODO: Safe way to add reference from CLR for smart pointer
	_baseComponent = GuiContext::GetInstance()->addWindow(marshal_as<std::string>(title)).get();
	boundingBox = gcnew GUICLR::BoundingBox(&_baseComponent->componentBoundingBox);
}

GUICLR::Component::~Component() {

}

void GUICLR::Component::addSlider(System::String^ title, float currentValue, float startValue, float endValue) {
	_baseComponent->addSlider(marshal_as<std::string>(title), currentValue, startValue, endValue);
}

void GUICLR::Component::setColor(int r, int g, int b) {
	_baseComponent->setColor(glm::vec3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f));
}

void GUICLR::Component::setTitle(System::String^ title) {
	_baseComponent->setTitle(marshal_as<std::string>(title));
}

System::String^ GUICLR::Component::getTitle() {
	return marshal_as<System::String^>(_baseComponent->getTitle());
}

GUICLR::BoundingBox^ GUICLR::Component::getBoundingBox() {
	return boundingBox;
}
