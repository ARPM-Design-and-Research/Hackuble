#pragma once

#include "Bezier.h"
#include "BezierRenderer.h"

Bezier::Bezier(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col, int res) {

	controlPoints[0] = v1;
	controlPoints[1] = v2;
	controlPoints[2] = v3;
	controlPoints[3] = v4;

	resolution = res;

	zDepth = 0.0f;

	color = col;
}

void Bezier::setColor(glm::vec3 col) {
	color = col;

	update = true;
}

void Bezier::setControlPoints(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4) {
	controlPoints[0] = v1;
	controlPoints[1] = v2;
	controlPoints[2] = v3;
	controlPoints[3] = v4;

	update = true;
}