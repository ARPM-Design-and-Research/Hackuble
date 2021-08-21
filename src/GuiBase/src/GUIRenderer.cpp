#pragma once

#include "GUIRenderer.h"

using namespace SynGUI;

GUIRenderer* GUIRenderer::guiRenderer_ = nullptr;

GUIRenderer* GUIRenderer::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (guiRenderer_ == nullptr) {
		guiRenderer_ = new GUIRenderer();
	}
	return guiRenderer_;
}

GUIRenderer::GUIRenderer() {

}

//Initialization of OpenGL states
void GUIRenderer::init(int width, int height) {

	/*We first render to an offscreen buffer which is twice the size of our screen resolution
	* and linearly scale it down to effectively get Super Sampling Anti Aliasing. The setup
	* for creating the framebuffer, attaching the color texture and Depth+Stencil texture. This 
	* allows for possibility of post-processing effects as well.*/
	glGenFramebuffers(1, &frameBufferObject);

	frameWidth = width;
	frameHeight = height;

	Camera::GetInstance()->setWindowSize(glm::vec2(width * 2, height * 2));

	GLCall(glGenTextures(1, &GUIRenderer::GetInstance()->screenTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, GUIRenderer::GetInstance()->screenTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * 2, height * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	framePixelsData = (GLubyte*)malloc(4 * width * height);

	GLCall(glGenRenderbuffers(1, &renderBufferObject));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width * 2, height * 2));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject));
	
	//Creating the vertex array, we basically render onto a quad which is the dimension of the screen
	GLCall(glGenVertexArrays(1, &screenVAO));
	GLCall(glBindVertexArray(screenVAO));

	signed char screenVertices[] = {
		-1,-1,0,0,
		1,1,1,1,
		-1,1,0,1,
		1,1,1,1,
		-1,-1,0,0,
		1,-1,1,0
	};

	GLCall(glGenBuffers(1, &screenVBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, screenVBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(signed char) * 4 * 6, screenVertices, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, sizeof(signed char) * 4, 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_BYTE, GL_FALSE, sizeof(signed char) * 4, (GLvoid*)(sizeof(signed char) * 2)));


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ASSERT(0);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//Compile the screenshader, it is a basic shader which renders onto a quad
	ShaderProgramSource screenSource = ParseShader("res/shaders/Screen.shader");
	screenShader = CreateShader(screenSource.VertexSource, screenSource.FragmentSource);

	//Call initialization on other renderer classes.
	Camera::GetInstance()->init();
	TextRenderer::GetInstance()->init();
	BezierRenderer::GetInstance()->init();
	RectangleRenderer::GetInstance()->init();
	IconRenderer::GetInstance()->init();
}

/* The render function basically lets the other renderers render onto
* an offscreen buffer. The screenshader is then used to take this offscreen buffer
* as a texture and render it onto a quad on the screen
*/
void GUIRenderer::render() {

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	GLCall(glViewport(0, 0, frameWidth * 2, frameHeight * 2));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


	RectangleRenderer::GetInstance()->render();
	TextRenderer::GetInstance()->render();
	BezierRenderer::GetInstance()->render();
	IconRenderer::GetInstance()->render();

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, frameWidth, frameHeight));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glUseProgram(screenShader));
	GLCall(glBindVertexArray(screenVAO));
	GLCall(int tex = glGetUniformLocation(screenShader, "uSampler"));
	ASSERT(tex != -1);
	GLCall(glUniform1i(tex, 0));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, screenTexture));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	glReadPixels(0, 0, frameWidth, frameHeight, GL_BGRA, GL_UNSIGNED_BYTE, framePixelsData);
}

GLubyte* GUIRenderer::getPixelData() {
	return framePixelsData;
}

//Update function, called each frame
void GUIRenderer::update() {
	Camera::GetInstance()->update();
}

/* When the screen size is changed, we have to reallocate the buffers with 
* the new screen resolution. This process is also memory intensive. GLFW
* screen resize callbacks issues too many calls to this function which causes
* a lot of unused memory allocations. 
* 
* It would be ideal to have just one call to this function
*/
void GUIRenderer::setScreenSize(int width, int height) {

	Camera::GetInstance()->setWindowSize(glm::vec2(width * 2, height * 2));

	free(framePixelsData);
	framePixelsData = (GLubyte*)malloc(4 * width * height);

	frameWidth = width;
	frameHeight = height;

	//Could cause problem on first call?
	//glDeleteTextures(1, &screenTexture);
	//glDeleteRenderbuffers(1, &renderBufferObject);

	glGenTextures(1, &screenTexture);
	GLCall(glBindTexture(GL_TEXTURE_2D, screenTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * 2, height * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	glGenRenderbuffers(1, &renderBufferObject);
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width * 2, height * 2));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

//Deinitialization and cleanup
void GUIRenderer::deinit() {

	RectangleRenderer::GetInstance()->deinit();
	BezierRenderer::GetInstance()->deinit();
	TextRenderer::GetInstance()->deinit();
	Camera::GetInstance()->deinit();
	IconRenderer::GetInstance()->deinit();

	glDeleteTextures(1, &screenTexture);
	glDeleteBuffers(1, &screenVBO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteRenderbuffers(1, &renderBufferObject);
	glDeleteProgram(screenShader);
	glDeleteFramebuffers(1, &frameBufferObject);

	//delete RectangleRenderer::GetInstance();
	//delete Camera::GetInstance();
	//delete TextRenderer::GetInstance();
	//delete BezierRenderer::GetInstance();
	//delete IconRenderer::GetInstance();

	free(framePixelsData);

	guiRenderer_ = nullptr;
	delete this;
}

