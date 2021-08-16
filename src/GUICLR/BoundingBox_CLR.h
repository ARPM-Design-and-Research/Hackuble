#pragma once

#include "GuiBase.h"

namespace GUICLR {
	public ref class BoundingBox {
	private:
		SynGUI::BoundingBox* _boundingBox;
	public:
		//TODO: Hide constructor
		BoundingBox(SynGUI::BoundingBox* bbBox);
		~BoundingBox();

		float getLeftBound();
		float getRightBound();
		float getTopBound();
		float getBottomBound();
	};
}
