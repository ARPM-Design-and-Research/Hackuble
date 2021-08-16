#pragma once

#include "BoundingBox_CLR.h"
#include "GuiContext.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace GUICLR {
	public ref class Component : public IDisposable {
	private:
		//TODO: Fix having direct pointer to baseComponent
		BaseWindow* _baseComponent;
		GUICLR::BoundingBox^ boundingBox;
	public:
		Component(System::String^ title);
		~Component();

		void addSlider(System::String^ title, float currentValue, float startValue, float endValue);
		void setColor(int r, int g, int b);
		void setTitle(System::String^ title);
		System::String^ getTitle();
		GUICLR::BoundingBox^ getBoundingBox();
	};
}
