#pragma once

#include "Slider.h"

using namespace SynGUI;

//TODO: Figure out a JSON format for describing graphic elements. It is mostly offsets and size relative to the slider position and size
Slider::Slider(const std::string& _label, glm::vec2 _pos, glm::vec2 _size, float _currentValue, float _startValue, float _endValue, float layer) {

	label = _label;
	pos = _pos;
	size = _size;

	currentValue = _currentValue;
	startValue = _startValue;
	endValue = _endValue;

	baseRectangle = RectangleRenderer::GetInstance()->addRectangle(pos, size, 0.0f, 2.0f, 0.0f, 0.0f, glm::vec3(51.0f / 255.0f), layer + 0.0001f);
	inputNode = RectangleRenderer::GetInstance()->addRectangle(glm::vec2(pos.x-6.0f, pos.y), glm::vec2(6.0f, size.y), 6.0f, 0.0f, 0.0f, 6.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0001f);

	float width = size.x * glm::clamp((currentValue + startValue) / (endValue - startValue), 0.0f, 1.0f);
	slideRectangle = RectangleRenderer::GetInstance()->addRectangle(pos, glm::vec2(width, size.y), 0.0f, 0.0f, 0.0f, 0.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0002f);

	titleLabel = TextRenderer::GetInstance()->addText(label, 0.11f, glm::vec2(0.0f, 0.0f));
	titleLabel->setBox(glm::vec2(pos.x + 3.0f, pos.y + 3.0f), glm::vec2(pos.x + 50.0f, pos.y + 9.0f));
	titleLabel->setZDepth(layer + 0.0003f);

	valueLabel = TextRenderer::GetInstance()->addText(floatToString(currentValue), 0.11f, glm::vec2(0.0f, 0.0f), Pivot::BOTTOM_RIGHT);
	glm::vec2 p1 = pos + size;
	valueLabel->setBox(glm::vec2(p1.x - 63.0f, pos.y+3.0f), glm::vec2(p1.x - 7.0f, pos.y+9.0f));
	valueLabel->setZDepth(layer + 0.0003f);
}

void Slider::setPosition(glm::vec2 _pos) {

	baseRectangle->setPosition(_pos);
	inputNode->setPosition(glm::vec2(_pos.x - 6.0f, _pos.y));
	slideRectangle->setPosition(_pos);

	titleLabel->setBox(glm::vec2(3.0f + _pos.x, 3.0f + _pos.y), glm::vec2(50.0f + _pos.x,9.0f + _pos.y));
	glm::vec2 p1 = _pos + size;
	valueLabel->setBox(glm::vec2(p1.x - 63.0f, 3.0f + _pos.y), glm::vec2(p1.x - 7.0f, 9.0f + _pos.y));
}

void Slider::calculateBoundingBox() {
	//TODO: Add boundingbox calculation
}

void Slider::translate(glm::vec2 translate) {
	baseRectangle->translate(translate);
	inputNode->translate(translate);
	slideRectangle->translate(translate);

	titleLabel->translate(translate);
	valueLabel->translate(translate);
}

void Slider::setValue(float value) {
	currentValue = value;

	float width = size.x * glm::clamp((currentValue + startValue) / (endValue - startValue), 0.0f, 1.0f);
	slideRectangle->setSize(glm::vec2(width, size.y));

	valueLabel->updateText(floatToString(currentValue));
}