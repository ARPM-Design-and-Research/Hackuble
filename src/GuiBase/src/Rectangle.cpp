#pragma once

#include "Rectangle.h"
#include "RectangleRenderer.h"

using namespace SynGUI;

Rectangle::Rectangle(glm::vec2 pos_, glm::vec2 size_, float r0_, float r1_, float r2_, float r3_, glm::vec3 color_, Pivot _pivot, float zDepth_) {
	pos = pos_;
	size = size_;
	r0 = r0_;
	r1 = r1_;
	r2 = r2_;
	r3 = r3_;
	color = color_;
	//zDepth = zDepth_;
	setZDepth(zDepth_);
	pivot = _pivot;
}

Rectangle::~Rectangle() {

}


void Rectangle::setColor(glm::vec3 color_) {
	color = color_;

	update = true;
}

void Rectangle::setWithPivot(glm::vec2 _pos, glm::vec2 _size) {
	switch (pivot) {
	case Pivot::TOP_LEFT:
		pos = _pos;
		break;
	case Pivot::TOP_RIGHT:
		pos = glm::vec2(_pos.x - _size.x, _pos.y);
		break;
	case Pivot::BOTTOM_LEFT:
		pos = glm::vec2(_pos.x, _pos.y - _size.y);
		break;
	case Pivot::BOTTOM_RIGHT:
		pos = glm::vec2(_pos.x - _size.x, _pos.y - _size.y);
		break;
	case Pivot::CENTER:
		pos = glm::vec2(_pos.x - _size.x / 2.0f, _pos.y - _size.y / 2.0f);
		break;
	}
}

void Rectangle::setPosition(glm::vec2 _pos) {

	pos = _pos;

	setWithPivot(pos, size);

	update = true;
}

void Rectangle::translate(glm::vec2 translate) {
	pos += translate;

	setWithPivot(pos, size);

	update = true;
}

void Rectangle::setSize(glm::vec2 _size) {
	size = _size;

	setWithPivot(pos, size);

	update = true;
}

void Rectangle::setRadius(float _r0, float _r1, float _r2, float _r3) {
	r0 = _r0;
	r1 = _r1;
	r2 = _r2;
	r3 = _r3;

	update = true;
}

void Rectangle::setPivot(Pivot _pivot) {
	pivot = _pivot;
	setWithPivot(pos, size);
	update = true;
}