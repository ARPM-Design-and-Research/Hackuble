#pragma once

#include "Icon_CLR.h"
#include "GuiContext.h"
#include <msclr\marshal_cppstd.h>

using namespace GUICLR;
using namespace msclr::interop;

GUICLR::Icon::Icon(String^ iconName, Vector2 pos, Vector2 size) {
	_icon = GuiContext::GetInstance()->getGUI()->addIcon(marshal_as<std::string>(iconName), glm::vec2(pos.X, pos.Y), glm::vec2(size.X, size.Y), SynGUI::Pivot::TOP_LEFT, 0.001f);
	_renderElement = _icon;
}


GUICLR::Icon::~Icon() {
	GuiContext::GetInstance()->getGUI()->removeIcon(_icon);
	delete _icon;
	_icon = nullptr;
}