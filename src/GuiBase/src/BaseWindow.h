#pragma once

#include "Component.h"
#include "RectangleRenderer.h"
#include "Rectangle.h"
#include "TextLabel.h"
#include "TextRenderer.h"
#include "RenderElement.h"
#include <string>
#include "glm.hpp"
#include "Slider.h"
#include "Icon.h"
#include "IconRenderer.h"

#include "Event.h"


/*The base window class is used to create a base for rendering a component
* It automatically calculates the correct dimensions and layering for the sub graphic
* elements. You can add sliders, change the label and set the position
* and updating the children elements is handled by the class
* 
*/

namespace SynGUI {

	class BaseWindow : public Component {
	private:
		Rectangle* backgroundRect;
		Rectangle* topRect;
		TextLabel* titleLabel;
		Icon* icon;

		float currentHeight;

		float layer;

		void calculateBoundingBox();

	public:

		BaseWindow(const std::string& _title, const std::string& icon, glm::vec2 _size = glm::vec2(120.0f, 70.0f), glm::vec2 _pos = glm::vec2(0.0f), glm::vec3 color = glm::vec3(1.0f), float layer = 0.0f);
		~BaseWindow();

		std::string title;
		glm::vec3 color;

		void setPosition(glm::vec2 _pos);
		void translate(glm::vec2 _trans);
		void setColor(glm::vec3 _color);

		std::shared_ptr<Slider> addSlider(const std::string& title, SliderState _sliderState, float currentValue = 0.0f, float startValue = 0.0f, float endValue = 1.0f);

		void OnMouseMove(std::shared_ptr<MouseEvent> eventArgs);

		void setTitle(std::string title);
		std::string getTitle();
	};
}

/*DLL_C_FUNCTION
BaseWindow* createWindow(char* title, char* icon, glm::vec2 size, glm::vec2 pos, glm::vec3 color, float layer);

DLL_C_FUNCTION
void destroyWindow(BaseWindow* ptr);

DLL_C_FUNCTION
void addSlider(BaseWindow* ptr, char* title, float currentValue, float startValue, float endValue);*/

