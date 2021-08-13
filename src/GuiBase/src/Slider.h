#pragma once

#include "Component.h"
#include <string>
#include "RectangleRenderer.h"
#include "Rectangle.h"
#include "TextLabel.h"
#include "TextRenderer.h"
#include "RenderElement.h"
#include "glm.hpp"
#include "BezierRenderer.h"
#include "Component.h"

#include "Helper.h"

/* The Slider class is a component which hold the data and logic
* to render sliders. Updating the position and value will automatically
* update the rendering.
*/

namespace SynGUI {

	class Slider : public Component {

		Rectangle* baseRectangle;
		Rectangle* inputNode;
		Rectangle* slideRectangle;

		TextLabel* valueLabel;
		TextLabel* titleLabel;

		float startValue;
		float endValue;
		float currentValue;

	public:

		Slider(const std::string& label, glm::vec2 _pos, glm::vec2 size, float _currentValue = 0.0f, float _startValue = 0.0f, float _endValue = 1.0f, float layer = 0.0f);

		void setPosition(glm::vec2 _pos);
		void translate(glm::vec2 translate);
		void setValue(float value);

		void calculateBoundingBox();

		void OnMouseDown(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseMove(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseUp(std::shared_ptr<MouseEvent> eventArgs);

		Bezier* bezierCurve = nullptr;
		bool inputSelected;

		std::string label;
	};
}
