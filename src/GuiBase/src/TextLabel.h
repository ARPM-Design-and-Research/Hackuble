#pragma once

#include "glm.hpp"
#include <string>
#include "RenderElement.h"


/* The text label class contains all the necessary members to 
* describe a piece of text.
* 
* The textlabel works a different from other renderElements. The position
* of the text is relative to the bounding box. The coordinates of the boundingbox
* are used for global positioninig of the text. In most cases, leaving the textLabel position
* to (0,0) is ideal and just updating the bounding box positions.
* 
* Helper functions are present to help wrapping the text inside a
* bounding box
*/

namespace SynGUI {
	class TextRenderer;
}

using namespace SynGUI;

class TextLabel : public RenderElement {
	friend class SynGUI::TextRenderer;

private:

	int index;

	std::string text;
	std::string updatedText;
	float fontSize;

	bool added = false;

public:

	TextLabel(const std::string& _text, float fontSize, glm::vec2 pos = glm::vec2(0.0f,0.0f), Pivot pivot = Pivot::BOTTOM_LEFT, float zDepth = 0.0f);


	//Calculates font size based on bounding box given
	void wrapHorizontalInBoundingBox();
	void setHorizontalWidth(float width);
	float getTotalWidth();

	void updateText(const std::string& text);
	void setFontSize(float fontSize);

	void setPosition(glm::vec2 _pos);
	void translate(glm::vec2 translate);

	std::string getCurrentText();
};