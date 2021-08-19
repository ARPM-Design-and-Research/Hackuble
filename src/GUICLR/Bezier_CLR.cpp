#pragma once

#include "Bezier_CLR.h"
#include "GuiContext.h"

GUICLR::Bezier::Bezier(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4, Color color) {
	_bezier = GuiContext::GetInstance()->getGUI()->addBezier(glm::vec2(v1.X, v1.Y), glm::vec2(v2.X, v2.Y), glm::vec2(v3.X, v3.Y), glm::vec2(v4.X, v4.Y), glm::vec3(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f), 20);
	_renderElement = _bezier;
}

GUICLR::Bezier::~Bezier() {
	GuiContext::GetInstance()->getGUI()->removeBezier(_bezier);
	_bezier = nullptr;
}

void GUICLR::Bezier::setColor(Color _color) {
	if(_bezier == nullptr)
		throw gcnew NullReferenceException("Bezier is not initialized or has been deleted");

	_bezier->setColor(glm::vec3(_color.R / 255.0f, _color.G / 255.0f, _color.B / 255.0f));
}

void GUICLR::Bezier::setControlPoints(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4) {
	if (_bezier == nullptr)
		throw gcnew NullReferenceException("Bezier is not initialized or has been deleted");

	_bezier->setControlPoints(glm::vec2(v1.X, v1.Y), glm::vec2(v2.X, v2.Y), glm::vec2(v3.X, v3.Y), glm::vec2(v4.X, v4.Y));
}

void GUICLR::Bezier::setThickness(float thickness) {
	if (_bezier == nullptr)
		throw gcnew NullReferenceException("Bezier is not initialized or has been deleted");

	_bezier->setThickness(thickness);
}