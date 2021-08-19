#pragma once

#include "GuiBase.h"
#include "GuiContext.h"
#include "RenderElement_CLR.h"

using namespace System;
using namespace System::Numerics;

namespace GUICLR {

	public enum class TextAlignment {
		LEFT,
		RIGHT,
		CENTER
	};

	public ref class Text : public RenderElement {
	private:
		SynGUI::TextLabel* _text;
	public:
		Text(String^ _text, Vector2 _pos, Vector2 _size, float _fontSize, TextAlignment alignment);
		~Text();
		
		void updateText(String^ text);
		void setFontSize(float fontSize);
		void setAlignment(TextAlignment textAlignment);

		void setPosition(Vector2 _pos);
		void translate(Vector2 translate);
		void setSize(Vector2 _size);
	};
}
