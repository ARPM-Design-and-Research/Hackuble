#pragma once

using namespace System;
using namespace System::Numerics;

#include "GuiBase.h"

namespace GUICLR {

	public ref class RenderElement {
	protected:
		SynGUI::RenderElement* _renderElement = nullptr;
	public:
		RenderElement();
		~RenderElement();

		//void translate(Vector2 translate);
		//void setPosition(Vector2 position);
		void setZDepth(float depth);
		void setVisible(bool visible);

		//void setBox(glm::vec2 p1, glm::vec2 p2);
	};
}