#pragma once

#include "BoundingBox_CLR.h"

using namespace GUICLR;

GUICLR::BoundingBox::BoundingBox(SynGUI::BoundingBox* bbBox) {
	_boundingBox = bbBox;
}

GUICLR::BoundingBox::~BoundingBox() {

}

float GUICLR::BoundingBox::getX() {
	return _boundingBox->x0;
}

float GUICLR::BoundingBox::getY() {
	return _boundingBox->y0;
}

float GUICLR::BoundingBox::getWidth() {
	return _boundingBox->x1 - _boundingBox->x0;
}

float GUICLR::BoundingBox::getHeight() {
	return _boundingBox->y1 - _boundingBox->y0;
}