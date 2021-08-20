#pragma once

#include "Icon.h"

using namespace SynGUI;
//Basic constructor for graphic element members
Icon::Icon(const std::string& name, glm::vec2 _pos, glm::vec2 _size, Pivot _pivot, float _zDepth) {
	pos = _pos;
	pivot = _pivot;
	//zDepth = _zDepth;
	setZDepth(_zDepth);
	iconName = name;
	size = _size;
}

void Icon::setWithPivot(glm::vec2 _pos, glm::vec2 _size) {
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

void Icon::setPosition(glm::vec2 _pos) {
	pos = _pos;
	setWithPivot(pos, size);
	update = true;
}

void Icon::setSize(glm::vec2 _size) {
	size = _size;
	setWithPivot(pos, size);
	update = true;
}

void Icon::translate(glm::vec2 translate) {
	pos += translate;
	setWithPivot(pos, size);
	update = true;
}

void Icon::setPivot(Pivot _pivot) {
	pivot = _pivot;
	setWithPivot(pos, size);
	update = true;
}