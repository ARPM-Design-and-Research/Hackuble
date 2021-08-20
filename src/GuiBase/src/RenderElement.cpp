#pragma once

#include "RenderElement.h"
#include <iostream>
#include "Camera.h"

using namespace SynGUI;

RenderElement::RenderElement() {

}

/*void RenderElement::setPosition(glm::vec2 _pos) {
	/*pos = _pos;
	
	float width = box.x1 - box.x0;
	float height = box.y1 - box.y0;

	box.x0 = pos.x;
	box.y0 = pos.y;
	box.x1 = pos.x + width;
	box.y1 = pos.y + height;

	update = true;
}

void RenderElement::translate(glm::vec2 translate) {
	/*pos += translate;

	float width = box.x1 - box.x0;
	float height = box.y1 - box.y0;

	box.x0 = pos.x;
	box.y0 = pos.y;
	box.x1 = pos.x + width;
	box.y1 = pos.y + height;

	update = true;
}*/

void RenderElement::setZDepth(float _depth) {
	zDepth = _depth;

	update = true;
}
/*void RenderElement::setBox(glm::vec2 c1, glm::vec2 c2) {
	box.x0 = c1.x;
	box.y0 = c1.y;

	box.x1 = c2.x;
	box.y1 = c2.y;

	update = true;
}*/

void RenderElement::setVisible(bool visible) {
	render = visible;
}