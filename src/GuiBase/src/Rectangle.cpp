#pragma once

#include "Rectangle.h"
#include "RectangleRenderer.h"

using namespace SynGUI;

Rectangle::Rectangle(glm::vec2 pos_, glm::vec2 size_, float r0_, float r1_, float r2_, float r3_, glm::vec3 color_, float zDepth_) {
	pos = pos_;
	size = size_;
	r0 = r0_;
	r1 = r1_;
	r2 = r2_;
	r3 = r3_;
	color = color_;
	zDepth = zDepth_;
}

Rectangle::~Rectangle() {

}


void Rectangle::setColor(glm::vec3 color_) {
	color = color_;

	update = true;
}



void Rectangle::setSize(glm::vec2 _size) {
	//TODO: Add pivot functionality
	size = _size;

	update = true;
}

void Rectangle::setRadius(float _r0, float _r1, float _r2, float _r3) {
	r0 = _r0;
	r1 = _r1;
	r2 = _r2;
	r3 = _r3;

	update = true;
}