#pragma once

#include "GuiBase.h"
#include "RenderElement_CLR.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Numerics;

namespace GUICLR {

	public ref class Rectangle : public RenderElement {
	private:
		SynGUI::Rectangle* _rectangle = nullptr;
	public:
		Rectangle(Vector2 pos, Vector2 size, float r1, float r2, float r3, float r4, GUICLR::Pivot pivot, Color color);
		~Rectangle();

		void setColor(Color color_);
		void setSize(Vector2 _size);
		void setRadius(float r0, float r1, float r2, float r3);
		void deleteRectangle();

		void setPosition(Vector2 pos);
		void translate(Vector2 translate);
		void setPivot(GUICLR::Pivot pivot);
	};
}
