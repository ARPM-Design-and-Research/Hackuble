#pragma once

#include "RenderElement_CLR.h"
#include "GuiBase.h"
using namespace System::Drawing;
using namespace System::Numerics;

namespace GUICLR {
	public ref class Bezier : public RenderElement {
	private:
		SynGUI::Bezier* _bezier = nullptr;
	public:
		Bezier(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4, Color color);
		~Bezier();

		void setColor(Color color);
		void setControlPoints(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4);
		void setThickness(float thickness);
	};
}
