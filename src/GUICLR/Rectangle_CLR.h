#pragma once

#include "GuiBase.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Numerics;

namespace GUICLR {

	public ref class RenderElement {
	protected:
		SynGUI::RenderElement* _renderElement = nullptr;
	public:
		RenderElement();
		~RenderElement();

		void translate(Vector2 translate);
		void setPosition(Vector2 position);
		void setZDepth(float depth);
		void setVisible(bool visible);

		//void setBox(glm::vec2 p1, glm::vec2 p2);
	};

	public ref class Rectangle : public RenderElement {
	private:
		SynGUI::Rectangle* _rectangle = nullptr;
	public:
		Rectangle();
		~Rectangle();

		void setColor(Color color_);
		void setSize(Vector2 _size);
		void setRadius(float r0, float r1, float r2, float r3);
		void deleteRectangle();
	};
}
