#pragma once

#include "Rectangle_CLR.h"
#include "GuiContext.h"

using namespace GUICLR;


GUICLR::RenderElement::RenderElement() {

}

GUICLR::RenderElement::~RenderElement() {
	_renderElement = nullptr;
}

void GUICLR::RenderElement::translate(Vector2 translate) {

	if (_renderElement == nullptr)
		throw gcnew NullReferenceException("Render Element is not initialized or has been deleted");

	_renderElement->translate(glm::vec2(translate.X,translate.Y));
}

void GUICLR::RenderElement::setPosition(Vector2 pos) {
	if (_renderElement == nullptr)
		throw gcnew NullReferenceException("Render Element is not initialized or has been deleted");

	_renderElement->setPosition(glm::vec2(pos.X,pos.Y));
}

void GUICLR::RenderElement::setZDepth(float depth) {
	if (_renderElement == nullptr)
		throw gcnew NullReferenceException("Render Element is not initialized or has been deleted");

	_renderElement->setZDepth(depth);
}

void GUICLR::RenderElement::setVisible(bool visible) {
	if (_renderElement == nullptr)
		throw gcnew NullReferenceException("Render Element is not initialized or has been deleted");

	_renderElement->setVisible(visible);
}

GUICLR::Rectangle::Rectangle() {
	_rectangle = GuiContext::GetInstance()->getGUI()->addRectangle(glm::vec2(0.0f), glm::vec2(100.0f), 2.0f, 2.0f, 2.0f, 2.0f, glm::vec3(1.0f), 0.001f);
	_renderElement = _rectangle;
}

//TODO: Check if this works
GUICLR::Rectangle::~Rectangle() {
	GuiContext::GetInstance()->getGUI()->removeRectangle(_rectangle);
	_rectangle = nullptr;
}

void GUICLR::Rectangle::deleteRectangle() {
	GuiContext::GetInstance()->getGUI()->removeRectangle(_rectangle);
	_rectangle = nullptr;
}

void GUICLR::Rectangle::setColor(Color _color) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->setColor(glm::vec3(_color.R/255.0f,_color.G/255.0f,_color.B/255.0f));
}

void GUICLR::Rectangle::setSize(Vector2 _size) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->setSize(glm::vec2(_size.X, _size.Y));
}

void GUICLR::Rectangle::setRadius(float r0, float r1, float r2, float r3) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->setRadius(r0, r1, r2, r3);
}