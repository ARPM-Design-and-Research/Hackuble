#pragma once

#include "GuiContext.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

#include "GuiBase.h"
#include "clr_scoped_ptr.h"


namespace GUICLR {
	public ref class ManagedContext
	{
	private:

	public:
		ManagedContext();
		~ManagedContext();

		void createContext(int windowWidth, int windowHeight);
		void resize(int newWidth, int newHeight);

		void getPixelData(array<unsigned char>^% pixelData);

		void onMouseMove(float x, float y, Int32 mouseButton);
		void onMouseDown(float x, float y, Int32 mouseButton);
		void onMouseUp(float x, float y, Int32 mouseButton);
		void onMouseWheel(float x, float y, int delta);

		void onPaint();

		void awaitInitialized();
		void awaitResize();

		void closeContext();
		// TODO: Add your methods for this class here.
	};
}



