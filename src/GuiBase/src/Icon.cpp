#pragma once

#include "Icon.h"

using namespace SynGUI;
//Basic constructor for graphic element members
Icon::Icon(const std::string& name, glm::vec2 _pos, glm::vec2 _size, Pivot _pivot, float _zDepth) {
	pos = _pos;
	pivot = _pivot;
	zDepth = _zDepth;
	iconName = name;

	//TODO: Add pivot functionality
	box.x0 = pos.x;
	box.y0 = pos.y;

	box.x1 = box.x0 + _size.x;
	box.y1 = box.y0 + _size.y;
}