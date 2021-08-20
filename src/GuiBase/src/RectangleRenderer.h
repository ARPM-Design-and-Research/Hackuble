#pragma once


#include <vector>
#include <iostream>

#include "Shaders.h"
#include "Error.h"
#include "Camera.h"


/* The RectangleRenderer class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call RectangleRenderer::GetInstance() to get the
* pointer to the instance.
* 
* To efficiently draw rounded rectangles without much tessellation, the approach from
* this blog post was used: https://mortoray.com/2015/06/05/quickly-drawing-a-rounded-rectangle-with-a-gl-shader/
* 
* All the neccessary data is packed into the vertex buffer and the rectangle is rendered using
* linear parametrization.
* 
* The RectangleRenderer class takes care of the rectangle vertice buffer, intialization,
* rendering and deinitialization.
*/

namespace SynGUI {
	class Rectangle;

	class RectangleRenderer {
	private:
		int shader_mvp;
		int shader_viewMatrix;
	protected:
		static RectangleRenderer* rectangleRenderer_;

		RectangleRenderer();
		void addRectangleToBuffer(Rectangle* rectangle);
		void updateBuffer();

		bool isUpdateBuffer = false;

	public:
		unsigned int vertexArrayObject;
		unsigned int vertexBuffer;
		unsigned int rectBuffer;

		//Have one global rectangle shader
		unsigned int RectangleShader;

		std::vector<Rectangle*> rectangles;
		std::vector<float> rectData;
		int currentBufferSize = 0;

		RectangleRenderer(RectangleRenderer& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const RectangleRenderer&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static RectangleRenderer* GetInstance();

		void init();
		void render();
		void deinit();

		Rectangle* addRectangle(glm::vec2 pos, glm::vec2 size, float r1, float r2, float r3, float r4, glm::vec3 color, Pivot pivot, float zDepth);
		void removeRectangle(Rectangle* rectangle);

		void updateRectangle(Rectangle* rect);
	};
}

