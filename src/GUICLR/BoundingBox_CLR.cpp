#pragma once

#include "BoundingBox_CLR.h"

using namespace GUICLR;

GUICLR::BoundingBox::BoundingBox(SynGUI::BoundingBox* bbBox) {
	_boundingBox = bbBox;
}

GUICLR::BoundingBox::~BoundingBox() {

}

float GUICLR::BoundingBox::getLeftBound() {
	return _boundingBox->x0;
}

float GUICLR::BoundingBox::getRightBound() {
	return _boundingBox->x1;
}

float GUICLR::BoundingBox::getTopBound() {
	return _boundingBox->y0;
}

float GUICLR::BoundingBox::getBottomBound() {
	return _boundingBox->y1;
}