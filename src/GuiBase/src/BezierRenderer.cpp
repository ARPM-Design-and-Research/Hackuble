#pragma once

#include "BezierRenderer.h"
#include "Bezier.h"

using namespace SynGUI;

BezierRenderer* BezierRenderer::bezierRenderer_ = nullptr;

BezierRenderer* BezierRenderer::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (bezierRenderer_ == nullptr) {
		bezierRenderer_ = new BezierRenderer();
	}
	return bezierRenderer_;
}

//Initialize shaders and buffers
void BezierRenderer::init() {

	ShaderProgramSource expSource = ParseShader("res/shaders/Bezier-Exp.shader");
	BezierShader = CreateShader(expSource.VertexSource, expSource.FragmentSource);

	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &SSBO);
}

BezierRenderer::BezierRenderer() {

	
}

/*Set the neccessary OpenGL statesand issue the draw call - called automatically by GUIRenderer
* 
*/
void BezierRenderer::render() {

	//Check for update tag and update the bezier curves
	for (int i = 0; i < bezierCurves.size(); i++) {
		if (bezierCurves.at(i)->added == false)
			addBezierToBuffer(bezierCurves.at(i));

		if (bezierCurves.at(i)->update)
			updateBezierCurve(bezierCurves.at(i));
	}

	glBindVertexArray(VAO);
	GLCall(glUseProgram(BezierShader));

	GLCall(int mvp = glGetUniformLocation(BezierShader, "u_mvp"));
	ASSERT(mvp != -1);
	GLCall(glUniformMatrix4fv(mvp, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));

	//TODO: set resolution automatically
	GLCall(int res = glGetUniformLocation(BezierShader, "u_resolution"));
	ASSERT(res != -1);
	GLCall(glUniform2f(res, 1280.0f * 2.0f, 720.0f * 2.0f));

	GLCall(int thick = glGetUniformLocation(BezierShader, "u_thickness"));
	ASSERT(thick != -1);
	GLCall(glUniform1f(thick, lineWidth));

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	int offset = 0;

	GLCall(int color = glGetUniformLocation(BezierShader, "u_color"));
	ASSERT(color != -1);

	for (int i = 0; i < bezierCurves.size(); i++) {
		
		GLsizei N1 = (GLsizei)bezierCurves.at(i)->resolution + 2;
		glm::vec3 c = bezierCurves.at(i)->color;

		GLCall(glUniform3f(color, c.x, c.y, c.z));
		glDrawArrays(GL_TRIANGLES, offset*6, 6 * (N1 - 1));

		offset += bezierCurves.at(i)->resolution + 3;
	}

}

//Internal function required to calculate bezier points
float getPt(float n1, float n2, float perc)
{
	float diff = n2 - n1;

	return n1 + (diff * perc);
}

/*Adds bezier points to the existing buffer.Calculations for the 
*intermediate points are done by using the 4 control points.
* 
* The buffer is an array of vec4 which has the x,y,z coordinates of
* all the points of the bezier curve. The res parameter defines how many
* points to have in one bezier curve.
* 
* The first point is pushed in once and the last point is pushed in twice.
* This is required for correct calculations in the shader
*/
void BezierRenderer::addBezierToBuffer(Bezier* _bezier) {

	Bezier* bezier = _bezier;

	glm::vec2 v1 = bezier->controlPoints[0];
	glm::vec2 v2 = bezier->controlPoints[1];
	glm::vec2 v3 = bezier->controlPoints[2];
	glm::vec2 v4 = bezier->controlPoints[3];

	bezierVertices.push_back(glm::vec4(v1.x, v1.y, bezier->zDepth, 1.0f));

	for (float i = 0; i < 1; i += 1.0f / ((float)bezier->resolution))
	{
		float xa = getPt(v1.x, v2.x, i);
		float ya = getPt(v1.y, v2.y, i);
		float xb = getPt(v2.x, v3.x, i);
		float yb = getPt(v2.y, v3.y, i);
		float xc = getPt(v3.x, v4.x, i);
		float yc = getPt(v3.y, v4.y, i);

		float xm = getPt(xa, xb, i);
		float ym = getPt(ya, yb, i);
		float xn = getPt(xb, xc, i);
		float yn = getPt(yb, yc, i);

		float x = getPt(xm, xn, i);
		float y = getPt(ym, yn, i);

		bezierVertices.push_back(glm::vec4(x, y, bezier->zDepth, 1.0f));
	}


	//Push last point in twice
	//So that GL_LINES_STRIP doesnt extend to next line
	bezierVertices.push_back(glm::vec4(v4.x, v4.y, bezier->zDepth, 1.0f));
	bezierVertices.push_back(glm::vec4(v4.x, v4.y, bezier->zDepth, 1.0f));

	glBindVertexArray(VAO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bezierVertices.size() * sizeof(glm::vec4), bezierVertices.data(), GL_DYNAMIC_DRAW);

	bezier->added = true;
}

/*Update BezierCurve is similar to adding a new bezier curve
* All the poins are recalculated.
* The only difference is instead of pushing the new points
* at the end of the buffer, they are changed in the existing buffer
* 
* This is called each time any Bezier curve has the update tag
* set to true
*/
void BezierRenderer::updateBezierCurve(Bezier* bezier) {

	int index = bezier->index;
	int offset = 0;

	for (int i = 0; i < index; i++) {
		offset += bezierCurves.at(i)->resolution+3;
	}
	
	glm::vec2 p1 = glm::vec2(bezier->controlPoints[0].x, bezier->controlPoints[0].y);
	glm::vec2 p2 = glm::vec2(bezier->controlPoints[1].x, bezier->controlPoints[1].y);
	glm::vec2 p3 = glm::vec2(bezier->controlPoints[2].x, bezier->controlPoints[2].y);
	glm::vec2 p4 = glm::vec2(bezier->controlPoints[3].x, bezier->controlPoints[3].y);

	int counter = 0;

	bezierVertices[offset] = glm::vec4(glm::vec4(p1.x, p1.y, bezier->zDepth, 1.0f));

	for (float i = 0; i < 1; i += 1.0f/((float)bezier->resolution))
	{
		float xa = getPt(p1.x, p2.x, i);
		float ya = getPt(p1.y, p2.y, i);
		float xb = getPt(p2.x, p3.x, i);
		float yb = getPt(p2.y, p3.y, i);
		float xc = getPt(p3.x, p4.x, i);
		float yc = getPt(p3.y, p4.y, i);

		float xm = getPt(xa, xb, i);
		float ym = getPt(ya, yb, i);
		float xn = getPt(xb, xc, i);
		float yn = getPt(yb, yc, i);

		float x = getPt(xm, xn, i);
		float y = getPt(ym, yn, i);

		bezierVertices[offset + 1 + counter] = glm::vec4(x, y, bezier->zDepth, 1.0f);

		counter++;
	}

	//TODO: Algorithm causes a problem with point V0 being at (0,0) or multiple of 30

	//Push last point in twice
	//So that GL_LINES_STRIP doesnt extend to next line
	bezierVertices[offset + counter + 1] = glm::vec4(p4.x,p4.y, bezier->zDepth, 1.0f);
	bezierVertices[offset + counter + 2] = glm::vec4(p4.x, p4.y, bezier->zDepth, 1.0f);

	glBindVertexArray(VAO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * offset, sizeof(glm::vec4) * (bezier->resolution+3), &bezierVertices[offset]);

	bezier->update = false;
}

Bezier* BezierRenderer::addBezierCurve(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4, glm::vec3 col, int res) {
	Bezier* bezier = new Bezier(v1, v2, v3, v4, col, res);
	bezier->index = bezierCurves.size();
	bezierCurves.push_back(bezier);

	//TODO: Remove this after solving zDepth
	bezier->zDepth = 0.005f;

	bezier->added = false;

	return bezier;
}

//Cleanup - this is called automatically by GUIRenderer
void BezierRenderer::deinit() {

	for (int i = 0; i < bezierCurves.size(); i++) {
		delete bezierCurves.at(i);
	}

	glDeleteProgram(BezierShader);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &SSBO);

	bezierRenderer_ = nullptr;
	delete this;
}

void BezierRenderer::setLineWidth(float width) {
	lineWidth = width;
}