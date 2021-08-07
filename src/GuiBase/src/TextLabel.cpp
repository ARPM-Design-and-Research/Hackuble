#pragma once

#include "TextLabel.h"
#include "TextRenderer.h"

TextLabel::TextLabel(const std::string& _text, float _fontSize, glm::vec2 _pos, Pivot _pivot, float _zDepth) {
	text = _text;
    updatedText = _text;
	pos = _pos;
	pivot = _pivot;
	zDepth = _zDepth;
    fontSize = _fontSize;

    //TODO: Add pivot functionality
    box.x0 = pos.x;
    box.y0 = pos.y;

    float totalWidth = getTotalWidth();

    box.x1 = box.x0 + totalWidth;
    box.y1 = box.y0 + TextRenderer::GetInstance()->atlasInfo.fontSize * fontSize;
}

//Adjusts the total width to fit inside bounding box
void TextLabel::wrapHorizontalInBoundingBox() {

    float totalWidth = getTotalWidth();

    float scaleFactor = (box.x1 - box.x0) / totalWidth;

    fontSize *= scaleFactor;

    update = true;
}

//Use this to set the width and thus the size of the text
void TextLabel::setHorizontalWidth(float width) {

    box.x1 = box.x0 + width;

    wrapHorizontalInBoundingBox();

    update = true;

}

//Calculates the totalWidth taken by the text in the current fontSize
float TextLabel::getTotalWidth() {
    const char* txt = text.c_str();

    float totalWidth = 0;

    //Calculate total width and skip over spaces (unicode 32)
    for (int i = 0; i < text.length(); i++) {
        GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

        if ((int)txt[i] == 32) {
            totalWidth += g1.advance * TextRenderer::GetInstance()->atlasInfo.fontSize * fontSize;
        }
        else if (g1.size.x != 0 && ((int)txt[i]) != 32) {
            totalWidth += (g1.advance) * TextRenderer::GetInstance()->atlasInfo.fontSize * fontSize;
        }
    }

    return totalWidth;
}

void TextLabel::updateText(const std::string& str) {
    //text = str;
    updatedText = str;
    update = true;
}

void TextLabel::setFontSize(float _fontSize) {
    fontSize = _fontSize;
    update = true;
}

void TextLabel::setPosition(glm::vec2 _pos) {
    //pos = _pos;

    float width = box.x1 - box.x0;
    float height = box.y1 - box.y0;

    setBox(glm::vec2(_pos.x,  _pos.y), glm::vec2(_pos.x+width, _pos.y + height));

    update = true;
}

void TextLabel::translate(glm::vec2 translate) {
    //pos += translate;

    float width = box.x1 - box.x0;
    float height = box.y1 - box.y0;

    setBox(glm::vec2(box.x0 + translate.x, box.y0 + translate.y), glm::vec2(box.x0 + translate.x + width, box.y0 + translate.y + height));

    update = true;
}