#pragma once

#include "TextLabel.h"
#include "TextRenderer.h"

namespace SynGUI {
    TextLabel::TextLabel(const std::string& _text, float _fontSize, glm::vec2 _pos, TextAlignment _alignment, Pivot _pivot, float _zDepth) {
        text = _text;
        updatedText = _text;
        pos = _pos;
        pivot = _pivot;
        zDepth = _zDepth;
        fontSize = _fontSize;
        textAlignment = _alignment;

        //TODO: Add pivot functionality
        box.x0 = _pos.x;
        box.y0 = _pos.y;

        float totalWidth = getTotalWidth();

        box.x1 = box.x0 + totalWidth;
        box.y1 = box.y0 + fontSize * 1.00f;
    }

    TextLabel::TextLabel(const std::string& _text, float _fontSize, glm::vec2 _pos, glm::vec2 _size, TextAlignment _alignment, float _zDepth) {
        text = _text;
        updatedText = _text;
        pos = _pos;
        pivot = Pivot::TOP_LEFT;
        zDepth = _zDepth;
        fontSize = _fontSize;
        textAlignment = _alignment;

        box.x0 = _pos.x;
        box.y0 = _pos.y;

        box.x1 = box.x0 + _size.x;
        box.y1 = box.y0 + _size.y;
    }

    void TextLabel::setAlignment(TextAlignment alignment) {
        textAlignment = alignment;

        update = true;
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
                totalWidth += g1.advance * fontSize;
            }
            else if (g1.size.x != 0 && ((int)txt[i]) != 32) {
                totalWidth += (g1.advance) * fontSize;
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

        box.x0 = _pos.x;
        box.y0 = _pos.y;
        box.x1 = _pos.x + width;
        box.y1 = _pos.y + height;

        update = true;
    }

    void TextLabel::setSize(glm::vec2 _size) {
        box.x1 = box.x0 + _size.x;
        box.y1 = box.y0 + _size.y;

        update = true;
    }

    void TextLabel::translate(glm::vec2 translate) {
        //pos += translate;

        float width = box.x1 - box.x0;
        float height = box.y1 - box.y0;

        box.x0 = box.x0 + translate.x;
        box.y0 = box.y0 + translate.y;
        box.x1 = box.x1 + translate.x;
        box.y1 = box.y1 + translate.y;


        update = true;
    }

    std::string TextLabel::getCurrentText() {
        return updatedText;
    }
}