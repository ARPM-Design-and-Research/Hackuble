#pragma once

#include "RenderElement.h"
#include "IconRenderer.h"

/* The Icon class is a data holder for storing icons or basically textures.
* It simply has the iconName as the unique identifier to each icon. This name
* needs to have the file name extension and needs to be correctly spelled.
* Internally, there is a map which stores the icon information against
* a key of string with the icon name. The wrong name will cause errors there.
*/

namespace SynGUI {
	class Icon : public RenderElement {
		friend class IconRenderer;

	private:
		int index;
		bool added = false;
		std::string iconName;

		glm::vec2 pos;
		glm::vec2 size;

		Icon(const std::string& iconName, glm::vec2 pos, glm::vec2 size, Pivot pivot, float zDepth);

	public:
		void setPosition(glm::vec2 pos);
		void translate(glm::vec2 translate);
	};
}