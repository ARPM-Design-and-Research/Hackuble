#pragma once

#include <GL/glew.h>

#include "Camera.h"
#include "Shaders.h"
#include "TextRenderer.h"
#include "BezierRenderer.h"
#include "RectangleRenderer.h"
#include "IconRenderer.h"

/* TThe GUIRenderer class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call GUIRenderer::GetInstance() to get the
* pointer to the instance.
*
* The GUIRenderer class handles all the OpenGL states, initialization and
* deinitialization of the other rendering classes. It calls the init, render
* and deinit functions of the other classes.
* 
* In the main program loop, just make sure that the init, update, render
* and deinit are called in the appropriate places.
*
*/

namespace SynGUI {

	class GUIRenderer {
	protected:

		static GUIRenderer* guiRenderer_;

		GUIRenderer();

		unsigned int screenTexture;
		unsigned int renderBufferObject;
		unsigned int frameBufferObject;

		unsigned int screenVAO;
		unsigned int screenVBO;

		unsigned int screenShader;

		GLubyte* framePixelsData;

	public:

		GUIRenderer(GUIRenderer& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const GUIRenderer&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static GUIRenderer* GetInstance();

		int frameWidth;
		int frameHeight;

		void init(int width, int height);
		void deinit();
		void render();
		void update();

		GLubyte* getPixelData();

		//static void setScreenSize(GLFWwindow* window, int width, int height);
		void setScreenSize(int width, int height);

	};
}