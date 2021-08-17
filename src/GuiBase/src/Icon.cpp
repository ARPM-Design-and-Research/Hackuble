#pragma once

#include "Icon.h"

using namespace SynGUI;
//Basic constructor for graphic element members
Icon::Icon(const std::string& name, glm::vec2 _pos, glm::vec2 _size, Pivot _pivot, float _zDepth) {
	pos = _pos;
	pivot = _pivot;
	zDepth = _zDepth;
	iconName = name;
	size = _size;
}

void Icon::setPosition(glm::vec2 _pos) {
	pos = _pos;

	update = true;
}

void Icon::translate(glm::vec2 translate) {
	pos += translate;

	update = true;
}