#pragma once

#include "glm.hpp"
#include "RenderElement.h"

/*A bezier class which holds information for a bezier curve
* All bezier curves are quadratic and so 4 vec2 points are stored
* in an array. The control points and color can be updated.
* 
* There is a way to render dashed lines and arrow lines by modifiying
* the fragment shader. High level functionality is yet to be added for that
*/

//TODO: Setup rendering styles for bezier curves

class BezierRenderer;

class Bezier : public RenderElement {

friend class BezierRenderer;

private:
	int index;

	glm::vec2 controlPoints[4];
	glm::vec3 color;

	bool added = false;
public:
	Bezier(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col = glm::vec3(1.0f), int res = 4);

	int resolution;

	void setColor(glm::vec3 col);
	void setControlPoints(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4);
};