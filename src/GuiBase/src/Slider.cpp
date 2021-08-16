#pragma once

#include "Slider.h"
#include "BezierRenderer.h"
#include "Bezier.h"

using namespace SynGUI;

//TODO: Figure out a JSON format for describing graphic elements. It is mostly offsets and size relative to the slider position and size
Slider::Slider(const std::string& _label, SliderState _sliderState, glm::vec2 _pos, glm::vec2 _size, float _currentValue, float _startValue, float _endValue, float layer) {

	label = _label;
	pos = _pos;
	size = _size;
	sliderState = _sliderState;

	currentValue = _currentValue;
	startValue = _startValue;
	endValue = _endValue;

	switch (sliderState) {
	case SliderState::INPUT:
	{
		baseRectangle = RectangleRenderer::GetInstance()->addRectangle(pos, size, 0.0f, 2.0f, 0.0f, 0.0f, glm::vec3(51.0f / 255.0f), layer + 0.0001f);
		inputNode = RectangleRenderer::GetInstance()->addRectangle(glm::vec2(pos.x - 6.0f, pos.y), glm::vec2(6.0f, size.y), 6.0f, 0.0f, 0.0f, 6.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0001f);

		float width = size.x * glm::clamp((currentValue + startValue) / (endValue - startValue), 0.0f, 1.0f);
		slideRectangle = RectangleRenderer::GetInstance()->addRectangle(pos, glm::vec2(width, size.y), 0.0f, 0.0f, 0.0f, 0.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0002f);

		titleLabel = TextRenderer::GetInstance()->addText(label, 0.11f, glm::vec2(0.0f, 0.0f));
		titleLabel->setBox(glm::vec2(pos.x + 3.0f, pos.y + 3.0f), glm::vec2(pos.x + 50.0f, pos.y + 9.0f));
		titleLabel->setZDepth(layer + 0.0003f);

		valueLabel = TextRenderer::GetInstance()->addText(floatToString(currentValue), 0.11f, glm::vec2(0.0f, 0.0f), Pivot::BOTTOM_RIGHT);
		glm::vec2 p1 = pos + size;
		valueLabel->setBox(glm::vec2(p1.x - 63.0f, pos.y + 3.0f), glm::vec2(p1.x - 7.0f, pos.y + 9.0f));
		valueLabel->setZDepth(layer + 0.0003f);
	}
		break;
		
	case SliderState::OUTPUT:
	{
		baseRectangle = RectangleRenderer::GetInstance()->addRectangle(glm::vec2(pos.x + 5.0f, pos.y), size, 2.0f, 0.0f, 0.0f, 0.0f, glm::vec3(51.0f / 255.0f), layer + 0.0001f);
		inputNode = RectangleRenderer::GetInstance()->addRectangle(glm::vec2(pos.x + size.x + 5.0f, pos.y), glm::vec2(6.0f, size.y), 0.0f, 6.0f, 6.0f, 0.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0001f);

		float width = size.x * glm::clamp((currentValue + startValue) / (endValue - startValue), 0.0f, 1.0f);
		slideRectangle = RectangleRenderer::GetInstance()->addRectangle(glm::vec2(pos.x + 5.0f, pos.y), glm::vec2(width, size.y), 2.0f, 0.0f, 0.0f, 0.0f, glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f), layer + 0.0002f);

		titleLabel = TextRenderer::GetInstance()->addText(label, 0.11f, glm::vec2(0.0f, 0.0f));
		titleLabel->setBox(glm::vec2(pos.x + 8.0f, pos.y + 3.0f), glm::vec2(pos.x + 55.0f, pos.y + 9.0f));
		titleLabel->setZDepth(layer + 0.0003f);

		valueLabel = TextRenderer::GetInstance()->addText(floatToString(currentValue), 0.11f, glm::vec2(0.0f, 0.0f), Pivot::BOTTOM_RIGHT);
		glm::vec2 p1 = pos + size;
		valueLabel->setBox(glm::vec2(p1.x - 58.0f, pos.y + 3.0f), glm::vec2(p1.x - 2.0f, pos.y + 9.0f));
		valueLabel->setZDepth(layer + 0.0003f);
	}
		break;
	}

	calculateBoundingBox();
}

void Slider::setPosition(glm::vec2 _pos) {

	baseRectangle->setPosition(_pos);
	inputNode->setPosition(glm::vec2(_pos.x - 6.0f, _pos.y));
	slideRectangle->setPosition(_pos);

	titleLabel->setBox(glm::vec2(3.0f + _pos.x, 3.0f + _pos.y), glm::vec2(50.0f + _pos.x,9.0f + _pos.y));
	glm::vec2 p1 = _pos + size;
	valueLabel->setBox(glm::vec2(p1.x - 63.0f, 3.0f + _pos.y), glm::vec2(p1.x - 7.0f, 9.0f + _pos.y));

	calculateBoundingBox();
}

void Slider::calculateBoundingBox() {

	switch (sliderState) {
	case SliderState::INPUT:
		componentBoundingBox.x0 = inputNode->box.x0;
		componentBoundingBox.y0 = inputNode->box.y0;
		componentBoundingBox.x1 = baseRectangle->box.x1;
		componentBoundingBox.y1 = baseRectangle->box.y1;
		break;
	case SliderState::OUTPUT:
		componentBoundingBox.x0 = baseRectangle->box.x0;
		componentBoundingBox.y0 = baseRectangle->box.y0;
		componentBoundingBox.x1 = inputNode->box.x1;
		componentBoundingBox.y1 = inputNode->box.y1;
		break;
	}
}

void Slider::translate(glm::vec2 translate) {
	baseRectangle->translate(translate);
	inputNode->translate(translate);
	slideRectangle->translate(translate);

	titleLabel->translate(translate);
	valueLabel->translate(translate);

	calculateBoundingBox();
}

void Slider::setValue(float value) {
	currentValue = value;

	float width = size.x * glm::clamp((currentValue + startValue) / (endValue - startValue), 0.0f, 1.0f);
	slideRectangle->setSize(glm::vec2(width, size.y));

	valueLabel->updateText(floatToString(currentValue));
}

void Slider::OnMouseDown(std::shared_ptr<MouseEvent> eventArgs) {

	if (eventArgs->pos.x > inputNode->box.x0 && eventArgs->pos.x < inputNode->box.x1 && eventArgs->pos.y > inputNode->box.y0 && eventArgs->pos.y < inputNode->box.y1) {
		inputSelected = true;

		if (bezierCurve == nullptr)
			bezierCurve = BezierRenderer::GetInstance()->addBezierCurve(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec3(65.0f / 255.0f, 169.0f / 255.0f, 213.0f / 255.0f));
	}
}

void Slider::OnMouseMove(std::shared_ptr<MouseEvent> eventArgs) {

	if (inputSelected && bezierCurve != nullptr) {
		glm::vec2 mousePos = glm::vec2(eventArgs->pos.x, eventArgs->pos.y);

		glm::vec2 v1 = glm::vec2((inputNode->box.x0 + inputNode->box.x1)/2.0f, (inputNode->box.y0 + inputNode->box.y1)/2.0f);
		glm::vec2 v2 = glm::vec2(mousePos.x, mousePos.y);
		glm::vec2 cp1, cp2;

		switch (sliderState) {
		case SliderState::INPUT:
			cp1 = glm::vec2(v1.x - glm::abs(v2.y - v1.y) * 1.6f, v1.y);
			cp2 = glm::vec2(v2.x + glm::abs(v2.y - v1.y) * 1.6f, v2.y);
			break;
		case SliderState::OUTPUT:
			cp1 = glm::vec2(v1.x + glm::abs(v2.y - v1.y) * 1.6f, v1.y);
			cp2 = glm::vec2(v2.x - glm::abs(v2.y - v1.y) * 1.6f, v2.y);
			break;
		}
		bezierCurve->setControlPoints(v1, cp1, cp2, v2);
	}
}

void Slider::OnMouseUp(std::shared_ptr<MouseEvent> eventArgs) {
	inputSelected = false;

	if (bezierCurve != nullptr) {
		BezierRenderer::GetInstance()->removeBezierCurve(bezierCurve);
		bezierCurve = nullptr;
	}
}