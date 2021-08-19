#pragma once

#include "Text_CLR.h"
#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;

namespace GUICLR {

	Text::Text(String^ _textStr, Vector2 _pos, Vector2 _size, float _fontSize, TextAlignment alignment) {
		_text = GuiContext::GetInstance()->getGUI()->addText(marshal_as<std::string>(_textStr), glm::vec2(_pos.X, _pos.Y), glm::vec2(_size.X, _size.Y), _fontSize, (SynGUI::TextAlignment)alignment);
		_renderElement = _text;
	}

	Text::~Text() {
		GuiContext::GetInstance()->getGUI()->removeText(_text);
	}

	void Text::updateText(String^ text) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->updateText(marshal_as<std::string>(text));
	}

	void Text::setFontSize(float fontSize) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->setFontSize(fontSize);
	}

	void Text::setAlignment(TextAlignment textAlignment) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->setAlignment((SynGUI::TextAlignment)textAlignment);
	}


	void Text::setPosition(Vector2 _pos) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->setPosition(glm::vec2(_pos.X, _pos.Y));
	}

	void Text::translate(Vector2 translate) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->translate(glm::vec2(translate.X, translate.Y));
	}

	void Text::setSize(Vector2 _size) {
		if (_text == nullptr)
			throw gcnew NullReferenceException("Text is not initialized or has been deleted");

		_text->setSize(glm::vec2(_size.X, _size.Y));
	}
}