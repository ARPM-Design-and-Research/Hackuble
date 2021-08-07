#pragma once

#include <GL/glew.h>
#include "Shaders.h"
#include "Camera.h"

#include "glm.hpp"
#include <vector>

/*The bezier renderer class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call BezierRenderer::GetInstance() to get the
* pointer to the instance.
* 
* The BezierRenderer stores all the bezier curves and has the relevant
* information in one contingous buffer. With the compiled shaders, a single
* draw call is issued to draw all the bezier curves
*/

class Bezier;

class BezierRenderer {
protected:

	static BezierRenderer* bezierRenderer_;

	void updateBezierCurve(Bezier* bezier);

	BezierRenderer();

public:
	
	BezierRenderer(BezierRenderer& other) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(const BezierRenderer&) = delete;
	/**
	 * This is the static method that controls the access to the singleton
	 * instance. On the first run, it creates a singleton object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */

	unsigned int BezierShader;
	unsigned int SSBO;
	unsigned int VAO;

	float lineWidth = 1.0f;

	static BezierRenderer* GetInstance();

	std::vector<Bezier*> bezierCurves;
	std::vector<glm::vec4> bezierVertices;

	//Create new bezier curve
	Bezier* addBezierCurve(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col = glm::vec3(1.0f), int res = 4);

	void addBezierToBuffer(Bezier* bezier);

	//Change line width of bezier curve
	void setLineWidth(float width);

	void init();
	void render();
	void deinit();
};