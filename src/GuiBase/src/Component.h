#pragma once

#include "glm.hpp"
#include <vector>
#include "Enum.h"

#include "Event.h"

#include <memory>
/* The Component class is a base class for all components.
*	
* A component can have child components, which are stored in a
* vector. Operations on the base components, like translation, trickle
* down to the child component.
* 
* To create a new component, you will need to derive it from this class.
*/

namespace SynGUI {

	class Component : public EventHandler {
	private:

	public:

		Component();
		~Component();

		glm::vec2 pos;
		glm::vec2 size;
		std::vector<std::shared_ptr<Component>> childComponents;

		virtual void setPosition(glm::vec2 pos);
		virtual void translate(glm::vec2 translate);

		BoundingBox componentBoundingBox;

		virtual void calculateBoundingBox();

		void addChildComponent(std::shared_ptr<Component> component);
	};
}
