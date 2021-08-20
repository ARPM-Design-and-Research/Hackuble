#pragma once

#include "RenderElement_CLR.h"
#include "GuiBase.h"

using namespace System;
using namespace System::Numerics;

namespace GUICLR {

	public ref class Icon : public RenderElement {
	private:
		SynGUI::Icon* _icon;
	public:
		Icon(String^ iconName, Vector2 pos, Vector2 size, GUICLR::Pivot pivot);
		~Icon();

		void setPosition(Vector2 pos);
		void translate(Vector2 translate);
		void setSize(Vector2 size);
		void setPivot(GUICLR::Pivot pivot);
	};
}
