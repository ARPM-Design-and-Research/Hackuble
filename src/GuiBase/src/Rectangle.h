#pragma once

#include "glm.hpp"
#include "RenderElement.h"

/*The Rectangle class hold all the neccessary data required for a rendering a
* rounded rectangle. Some of the bounding box functionality for the graphic element is
* also handled by this class as it is specific to rectangles.
* 
* For now, the position refers to the top left corner of the rectangle. The size refers to the width
* and height of the rectangle.
*/
namespace SynGUI {
	class RectangleRenderer;

	class Rectangle : public RenderElement {

		friend class RectangleRenderer;

	private:
		int index;
		Rectangle(glm::vec2 pos_, glm::vec2 size_, float r0_, float r1_, float r2_, float r3_, glm::vec3 color_, float _zDepth);
		~Rectangle();

		glm::vec2 size;
		float r0, r1, r2, r3;
		glm::vec3 color;

		bool added = false;

	public:
		void setColor(glm::vec3 color_);
		void setSize(glm::vec2 size_);
		void setRadius(float r0, float r1, float r2, float r3);
	};
}