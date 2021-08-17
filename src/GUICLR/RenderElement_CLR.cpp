#pragma once

#include "RenderElement_CLR.h"
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

	_renderElement->translate(glm::vec2(translate.X, translate.Y));
}

void GUICLR::RenderElement::setPosition(Vector2 pos) {
	if (_renderElement == nullptr)
		throw gcnew NullReferenceException("Render Element is not initialized or has been deleted");

	_renderElement->setPosition(glm::vec2(pos.X, pos.Y));
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