#pragma once

#include "glm.hpp"
#include "Enum.h"

/* The render element class is a base class from which all
* the render elements are derived : Rectangle, TextLabel, Icon, Bezier.
* 
* It contains data that every rendering object would require.
* 
* The update flag is used to notify the renderer that the particular
* renderElement has been updated and that the buffers in the gpu need to
* be updated as well.
* 
* BoundingBox logic is also taken care of with this class, but it is recommended
* to ovveride the functionality to fit the specific use case
*/

namespace SynGUI {

	class RenderElement {
	protected:

		bool update = false;
		glm::vec2 pos;
		Pivot pivot;

		float zDepth;

	public:
		RenderElement();

		void setPosition(glm::vec2 pos);
		void translate(glm::vec2 translate);
		void setZDepth(float depth);

		void setBox(glm::vec2 c1, glm::vec2 c2);
		BoundingBox box;
		//TODO: Add set Size
	};
}