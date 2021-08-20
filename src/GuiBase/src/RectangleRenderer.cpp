#pragma once

#include "RectangleRenderer.h"
#include "Rectangle.h"

using namespace SynGUI;

RectangleRenderer* RectangleRenderer::rectangleRenderer_ = nullptr;

RectangleRenderer* RectangleRenderer::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (rectangleRenderer_ == nullptr) {
		rectangleRenderer_ = new RectangleRenderer();
	}

	return rectangleRenderer_;
}



//Mapping - { r0, r1, r2, r3, X, Y, mn, 0, corner0, corner1, corner2, corner3, edge0, edge1, edge2, edge3}
//Defines all vertices needed to render rounded rectangle with linearized parameters

/* Essentially two buffers exist and are sent to the gpu.
* One contains static data (rectVerts) which is used for linear parameterization
* on the gpu. The rectBuffer contains rectangle specific data and is sent once every instance.
*/

static signed char rectVerts[24 * (6 * 8 + 3 * 8)] = {
	//Corner 0
	0, 0, 0, 0, 0, 0, 0, 0,  // (0,0)
	0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  //(0,r0)
	1, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  //(r0,r0)
	1, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,  // (0,0)
	0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  //(r0,r0)
	1, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  // (r0,0)
	0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0,


	//Corner 1
	0, -1, 0, 0, 1, 0, 0, 0,  //(X-r1,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0,  //(X-r1,r1)
	0, 1, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0,  // (X,0) 
	0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 1, 0, 0, 0,  // (X,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0,  //(X-r1,r1)
	0, 1, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0,  //(X,r1)
	0, 1, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,

	//Corner 2
	0,0,-1,0,1,0,0,0, //(X-r2,Y-r2)
	0,0,-1,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,
	0,0,0,0,1,0,0,0, //(X,Y)
	0,0,0,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,
	0,0,0,0,1,0,0,0, //(X,Y-r2)
	0,0,-1,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,

	0,0,-1,0,1,0,0,0, //(X-r2,Y-r2)
	0,0,-1,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,
	0,0,-1,0,1,0,0,0, //(X-r2,Y)
	0,0,0,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,
	0,0,0,0,1,0,0,0, //(X,Y)
	0,0,0,0,0,1,0,0, 0, 0, 1, 0, 0, 0, 0, 0,

	//Corner 3
	0,0,0,0,0,0,0,0, //(0,Y-r3)
	0,0,0,-1,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,
	0,0,0,0,0,0,0,0, //(0,Y)
	0,0,0,0,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,
	0,0,0,1,0,0,0,0, //(r3,Y-r3)
	0,0,0,-1,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,

	0,0,0,1,0,0,0,0, //(r3,Y-r3)
	0,0,0,-1,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,
	0,0,0,0,0,0,0,0, //(0,Y)
	0,0,0,0,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,
	0,0,0,1,0,0,0,0, //(r3,Y)
	0,0,0,0,0,1,0,0, 0, 0, 0, 1, 0, 0, 0, 0,

	//Tri 1
	0, 0, 0, 0, 0, 0, 1, 0,  //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  // (r0,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  //(r0,r0)
	1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,

	//Tri 2
	0, 0, 0, 0, 0, 0, 1, 0,  //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0, //(X-r1,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0,  // (r0,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,

	//Tri 3
	0, 0, 0, 0, 0, 0, 1, 0,  //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0, //(X-r1,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,

	//Tri 4
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0, //(X-r1,r1)
	0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,
	0, -1, 0, 0, 1, 0, 0, 0, //(X-r1,0)
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,


	//Tri 5
	0,0,0,0,0,0,1,0, //(mn,Y-mn)
	0,0,0,0,0,1,-1,0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, //(r3,Y-r3)
	0, 0, 0,-1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, //(r3,Y)
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,

	//Tri 6
	0, 0, 0, 0, 1, 0, -1, 0,//(X-mn, Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, -1, 0, 1, 0, 0, 0, //(X-r2,Y)
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, -1, 0, 1, 0, 0, 0, //(X-r2,Y-r2)
	0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,

	//Tri 7
	0, 0, 0, 0, 0, 0, 1, 0, //(mn,Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, //(r3,Y)
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn, Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0,

	//Tri 8
	0, 0, 0, 0, 1, 0, -1, 0,//(X-mn, Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, //(r3,Y)
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, -1, 0, 1, 0, 0, 0, //(X-r2,Y)
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,

	//Tri 9
	1, 0, 0, 0, 0, 0, 0, 0, //(r0,r0)
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, //(0,r0)
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0, //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,

	//Tri 10
	0, 0, 0, 0, 0, 0, 0, 0, //(0,Y-r3)
	0, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 1, 0, 0, 0, 0, //(r3, Y-r3)
	0, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0, //(mn, Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0,

	//Tri 11
	0, 0, 0, 0, 0, 0, 0, 0, //(0,r0)
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, //(0,Y-r3)
	0, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0, //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,

	//Tri 12
	0, 0, 0, 0, 0, 0, 1, 0, //(mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, //(0,Y-r3)
	0, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0, //(mn, Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0,

	//Tri 13
	0,-1, 0, 0, 1, 0, 0, 0, //(X-r1,r1)
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, 0, 0, //(X,r1)
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,

	//Tri 14
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, -1, 0, 1, 0, 0, 0, //(X-r2,Y-r2)
	0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, 0, 0, //(X,Y-r2)
	0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,

	//Tri 15
	0, 0, 0, 0, 1, 0, 0, 0, //(X,r1)
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,mn)
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1,

	//Tri 16
	0, 0, 0, 0, 1, 0, 0, 0, //(X,r1)
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, -1, 0, //(X-mn,Y-mn)
	0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, 0, 0, //(X,Y-r2)
	0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,

};



RectangleRenderer::RectangleRenderer() {

}

/* Initialize all the required buffer and specifiy the layout used to the VAO
*/
void RectangleRenderer::init() {

	ShaderProgramSource source = ParseShader("res/shaders/Rectangle.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	RectangleShader = shader;

	glUseProgram(RectangleShader);

	GLCall(shader_mvp = glGetUniformLocation(RectangleShader, "u_MVP"));
	ASSERT(shader_mvp != -1);

	GLCall(shader_viewMatrix = glGetUniformLocation(RectangleShader, "u_MV"));
	ASSERT(shader_viewMatrix != -1);

	//Generate VAO and VBO and bind attrib pointers
	GLCall(glGenVertexArrays(1, &vertexArrayObject));
	GLCall(glBindVertexArray(vertexArrayObject));

	GLCall(glGenBuffers(1, &vertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(signed char) * 24 * (6 * 8 + 3 * 8), &rectVerts[0], GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glEnableVertexAttribArray(4));
	GLCall(glEnableVertexAttribArray(5));

	/* code for sending static rectangle data
	*/

	//For X
	// 0, 0, 0, 0, - Defines r0, r1, r2,r3
	GLCall(glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, 0));
	// 0, 0, 0, 0, - Defines X, Y, mn, 0
	GLCall(glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, (GLvoid*)(sizeof(signed char) * 4)));

	//For Y
	// 0, 0, 0, 0, - Defines r0, r1, r2,r3
	GLCall(glVertexAttribPointer(2, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, (GLvoid*)(sizeof(signed char) * 8)));
	// 0, 0, 0, 0, - Defines X, Y, mn, 0
	GLCall(glVertexAttribPointer(3, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, (GLvoid*)(sizeof(signed char) * 12)));

	//At end
	// 0, 0, 0, 0, - Defines radius 0, 1, 2, 3
	GLCall(glVertexAttribPointer(4, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, (GLvoid*)(sizeof(signed char) * 16)));
	// 0, 0, 0, 0, - Defines edge 0, 1, 2, 3
	GLCall(glVertexAttribPointer(5, 4, GL_BYTE, GL_FALSE, sizeof(signed char) * 24, (GLvoid*)(sizeof(signed char) * 20)));

	GLCall(glGenBuffers(1, &rectBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rectBuffer));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 0);
	glVertexAttribDivisor(6, 1);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribDivisor(7, 1);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribDivisor(8, 1);

}


/* The render call is simple. It sets the neccessary OpenGL states,
* passes the variables to the shader and issues the draw call.
* 
* All rectangles are drawn using a single draw call.
*/
void RectangleRenderer::render() {

	//Checks to see if rectangle has update flag
	for (int i = 0; i < rectangles.size(); i++) {

		if (rectangles.at(i)->added == false)
			addRectangleToBuffer(rectangles.at(i));

		if (rectangles.at(i)->update)
			updateRectangle(rectangles.at(i));
	}

	if (isUpdateBuffer)
		updateBuffer();

	GLCall(glBindVertexArray(vertexArrayObject));
	GLCall(glUseProgram(RectangleShader));

	GLCall(glUniformMatrix4fv(shader_mvp, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));
	GLCall(glUniformMatrix4fv(shader_viewMatrix, 1, GL_FALSE, &Camera::GetInstance()->getViewMatrix()[0][0]));

	int continousCount = 0;
	int startIndex = 0;

	for (int i = 0; i < rectangles.size(); i++) {
		if (rectangles.at(i)->render) {
			continousCount++;
		}
		else {
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 72, continousCount, startIndex);
			startIndex = i + 1;
			continousCount = 0;
		}

		if (i == rectangles.size() - 1) {
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 72, continousCount, startIndex);
		}
	}
}

/* Create a new rectangle with the specific parameters, adds it to the buffer
* and sends it to the gpu.
*/
void RectangleRenderer::addRectangleToBuffer(Rectangle* rectangle) {

	Rectangle* rect = rectangle;

	rectData.push_back(rect->pos.x);
	rectData.push_back(rect->pos.y);
	rectData.push_back(rect->size.x);
	rectData.push_back(rect->size.y);
	rectData.push_back(rect->r0);
	rectData.push_back(rect->r1);
	rectData.push_back(rect->r2);
	rectData.push_back(rect->r3);
	rectData.push_back(rect->color.r);
	rectData.push_back(rect->color.g);
	rectData.push_back(rect->color.b);
	rectData.push_back(rect->zDepth);

	isUpdateBuffer = true;

	rect->added = true;
}

//Cleanup
void RectangleRenderer::deinit() {

	GLCall(glDeleteProgram(RectangleShader));
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &rectBuffer);

	for (int i = 0; i < rectangles.size(); i++) {
		delete rectangles.at(i);
	}

	rectangles.clear();
	rectData.clear();

	rectangleRenderer_ = nullptr;
	delete this;
}


/* When any rectangle is updated, its corresponding buffer is also
* updated and then sent to the gpu.
*/
void RectangleRenderer::updateRectangle(Rectangle* rectangle) {
	int index = rectangle->index;
	
	rectData[index * 12 + 0] = rectangle->pos.x;
	rectData[index * 12 + 1] = rectangle->pos.y;
	rectData[index * 12 + 2] = rectangle->size.x;
	rectData[index * 12 + 3] = rectangle->size.y;
	rectData[index * 12 + 4] = rectangle->r0;
	rectData[index * 12 + 5] = rectangle->r1;
	rectData[index * 12 + 6] = rectangle->r2;
	rectData[index * 12 + 7] = rectangle->r3;
	rectData[index * 12 + 8] = rectangle->color.r;
	rectData[index * 12 + 9] = rectangle->color.g;
	rectData[index * 12 + 10] = rectangle->color.b;
	rectData[index * 12 + 11] = rectangle->zDepth;

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER,rectBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(float) * 12, sizeof(float) * 12, &rectData[index*12]);

	rectangle->update = false;
}

void RectangleRenderer::removeRectangle(Rectangle* rectangle) {
	int index = rectangle->index;

	if (rectangle->added) {
		int bufferOffset = 0;
		for (int i = 0; i < index; i++) {
			bufferOffset += 12;
		}

		rectData.erase(rectData.begin() + bufferOffset, rectData.begin() + bufferOffset + 12);
	}

	rectangles.erase(rectangles.begin() + index);

	for (int i = index; i < rectangles.size(); i++) {
		rectangles.at(i)->index--;
	}

	isUpdateBuffer = true;

	delete rectangle;
}

void RectangleRenderer::updateBuffer() {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rectData.size(), rectData.data(), GL_DYNAMIC_DRAW);

	isUpdateBuffer = false;
}

Rectangle* RectangleRenderer::addRectangle(glm::vec2 pos, glm::vec2 size, float r1, float r2, float r3, float r4, glm::vec3 color, Pivot pivot, float zDepth) {
	Rectangle* rect = new Rectangle(pos, size, r1, r2, r3, r4, color, pivot, zDepth);
	rect->index = rectangles.size();
	rect->added = false;

	rectangles.push_back(rect);

	return rect;
}