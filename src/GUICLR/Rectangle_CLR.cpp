#pragma once

#include "Rectangle_CLR.h"
#include "GuiContext.h"

GUICLR::Rectangle::Rectangle(Vector2 pos, Vector2 size, float r1, float r2, float r3, float r4, GUICLR::Pivot pivot, Color color) {
	_rectangle = GuiContext::GetInstance()->getGUI()->addRectangle(glm::vec2(pos.X,pos.Y), glm::vec2(size.X,size.Y), r1, r2, r3, r4, glm::vec3(color.R/255.0f,color.G/255.0f,color.B/255.0f), (SynGUI::Pivot)pivot, 0.001f);
	_renderElement = _rectangle;
}

//TODO: Check if this works
GUICLR::Rectangle::~Rectangle() {
	GuiContext::GetInstance()->getGUI()->removeRectangle(_rectangle);
	_rectangle = nullptr;
}

void GUICLR::Rectangle::deleteRectangle() {
	GuiContext::GetInstance()->getGUI()->removeRectangle(_rectangle);
	delete _rectangle;
	_rectangle = nullptr;
}

void GUICLR::Rectangle::setPosition(Vector2 position) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->setPosition(glm::vec2(position.X, position.Y));
}

void GUICLR::Rectangle::translate(Vector2 trans) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->translate(glm::vec2(trans.X, trans.Y));
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

void GUICLR::Rectangle::setPivot(GUICLR::Pivot pivot) {
	if (_rectangle == nullptr)
		throw gcnew NullReferenceException("Rectangle is not initialized or has been deleted");

	_rectangle->setPivot((SynGUI::Pivot)pivot);
}