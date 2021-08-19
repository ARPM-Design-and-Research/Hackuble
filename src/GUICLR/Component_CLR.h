#pragma once

#include "BoundingBox_CLR.h"
#include "GuiContext.h"
#include "clr_scoped_ptr.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace GUICLR {

	public enum class SliderState {
		INPUT,
		OUTPUT,
		NONE
	};

	public ref class Component : public IDisposable {
	private:
		//TODO: Fix having direct pointer to baseComponent
		SynGUI::BaseWindow* _baseComponent;
		GUICLR::BoundingBox^ boundingBox;
	public:
		Component(System::String^ title);
		~Component();

		void addSlider(System::String^ title, GUICLR::SliderState _sliderState, float currentValue, float startValue, float endValue);
		void setColor(int r, int g, int b);
		void setTitle(System::String^ title);
		System::String^ getTitle();
		GUICLR::BoundingBox^ getBoundingBox();
	};
}
