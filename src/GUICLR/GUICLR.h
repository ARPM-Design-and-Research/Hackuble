#pragma once

#include "GuiContext.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

#include "GuiBase.h"


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
		void onMouseWheel(int delta);

		void onPaint();

		void awaitInitialized();
		void awaitResize();
		// TODO: Add your methods for this class here.
	};

	public ref class BoundingBox {
	public:
		BoundingBox();
		~BoundingBox();

		float x0;
		float y0;
		float x1;
		float y1;
	};

	public ref class Component {
	private:
		SynGUI::BaseWindow* baseComponent;
		GUICLR::BoundingBox boundingBox;
	public:
		Component(System::String^ title);
		~Component();

		void addSlider(System::String^ title, float currentValue, float startValue, float endValue);
		void setColor(float r, float g, float b);
		void setTitle(System::String^ title);
		System::String^ getTitle();
		//GUICLR::BoundingBox getBoundingBox();
	};

	public ref class Slider {
	public:
		Slider();
		~Slider();
	};
}



