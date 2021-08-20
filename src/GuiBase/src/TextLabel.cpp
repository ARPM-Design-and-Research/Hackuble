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

        size = glm::vec2(box.x1 - box.x0, box.y1 - box.y0);
    }

    TextLabel::TextLabel(const std::string& _text, float _fontSize, glm::vec2 _pos, glm::vec2 _size, TextAlignment _alignment, Pivot _pivot, float _zDepth) {
        text = _text;
        updatedText = _text;
        pos = _pos;
        pivot = _pivot;
        zDepth = _zDepth;
        fontSize = _fontSize;
        textAlignment = _alignment;

        box.x0 = _pos.x;
        box.y0 = _pos.y;

        box.x1 = box.x0 + _size.x;
        box.y1 = box.y0 + _size.y;
        
        size = _size;
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
        pos = _pos;

        setWithPivot(pos, size);

        /*float width = box.x1 - box.x0;
        float height = box.y1 - box.y0;

        box.x0 = _pos.x;
        box.y0 = _pos.y;
        box.x1 = _pos.x + width;
        box.y1 = _pos.y + height;*/

        update = true;
    }

    void TextLabel::setSize(glm::vec2 _size) {

        size = _size;

        setWithPivot(pos, size);

        /*box.x1 = box.x0 + _size.x;
        box.y1 = box.y0 + _size.y;*/

        update = true;
    }

    void TextLabel::translate(glm::vec2 translate) {
        pos += translate;

        setWithPivot(pos, size);

        /*float width = box.x1 - box.x0;
        float height = box.y1 - box.y0;

        box.x0 = box.x0 + translate.x;
        box.y0 = box.y0 + translate.y;
        box.x1 = box.x1 + translate.x;
        box.y1 = box.y1 + translate.y;*/


        update = true;
    }

    std::string TextLabel::getCurrentText() {
        return updatedText;
    }

    void TextLabel::setWithPivot(glm::vec2 _pos, glm::vec2 _size) {
        switch (pivot) {
        case Pivot::TOP_LEFT:
            pos = _pos;
            break;
        case Pivot::TOP_RIGHT:
            pos = glm::vec2(_pos.x - _size.x, _pos.y);
            break;
        case Pivot::BOTTOM_LEFT:
            pos = glm::vec2(_pos.x, _pos.y - _size.y);
            break;
        case Pivot::BOTTOM_RIGHT:
            pos = glm::vec2(_pos.x - _size.x, _pos.y - _size.y);
            break;
        case Pivot::CENTER:
            pos = glm::vec2(_pos.x - _size.x / 2.0f, _pos.y - _size.y / 2.0f);
            break;
        }

        box.x0 = pos.x;
        box.y0 = pos.y;
        box.x1 = pos.x + size.x;
        box.y1 = pos.y + size.y;
    }

    void TextLabel::setPivot(Pivot _pivot) {
        pivot = _pivot;
        setWithPivot(pos, size);
        update = true;
    }
}