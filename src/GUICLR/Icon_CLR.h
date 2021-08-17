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
		Icon(String^ iconName, Vector2 pos, Vector2 size);
		~Icon();

	};
}
