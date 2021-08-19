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

namespace SynGUI {
	class TextLabel : public RenderElement {
		friend class SynGUI::TextRenderer;

	private:

		int index;
		int bufferOffset;

		std::string text;
		std::string updatedText;
		float fontSize;

		bool added = false;

	public:

		TextLabel(const std::string& _text, float fontSize, glm::vec2 pos = glm::vec2(0.0f, 0.0f), TextAlignment alignment = TextAlignment::LEFT, Pivot pivot = Pivot::TOP_LEFT, float zDepth = 0.0f);
		TextLabel(const std::string& _text, float _fontSize, glm::vec2 _pos, glm::vec2 _size, TextAlignment alignment = TextAlignment::LEFT, float _zDepth = 0.0f);


		glm::vec2 pos;
		BoundingBox box;

		TextAlignment textAlignment;

		//Calculates font size based on bounding box given
		void wrapHorizontalInBoundingBox();
		void setHorizontalWidth(float width);
		float getTotalWidth();

		void updateText(const std::string& text);
		void setFontSize(float fontSize);
		void setAlignment(TextAlignment textAlignment);

		void setPosition(glm::vec2 _pos);
		void translate(glm::vec2 translate);
		void setSize(glm::vec2 _size);

		std::string getCurrentText();
	};
}