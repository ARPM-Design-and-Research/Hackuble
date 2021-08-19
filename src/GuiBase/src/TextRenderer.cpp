#pragma once

#include "TextRenderer.h"
#include "TextLabel.h"

#include "nlohmann/json.hpp"
#include "JsonParserAtlas.h"

#include <stdio.h>

namespace SynGUI {

    TextRenderer* TextRenderer::textRenderer_ = nullptr;

    TextRenderer::TextRenderer() {

    }

    TextRenderer* TextRenderer::GetInstance()
    {
        /**
         * This is a safer way to create an instance. instance = new Singleton is
         * dangeruous in case two instance threads wants to access at the same time
         */
        if (textRenderer_ == nullptr) {
            textRenderer_ = new TextRenderer();
        }
        return textRenderer_;
    }


    /* Initializes the required buffers and atlas textures.
    * Also parses the .json metadata into the appropriate structs
    */
    void TextRenderer::init() {

        //TODO: Add functionality to load fonts dynamically

        std::ifstream i("fonts/atlas.json");
        nlohmann::json j;
        i >> j;

        quicktype::FontData data = nlohmann::json::parse(j.dump());

        quicktype::Atlas atlas = data.get_atlas();

        atlasInfo.size = glm::vec2(atlas.get_width(), atlas.get_height());
        atlasInfo.fontSize = atlas.get_size();

        std::vector<quicktype::Glyph> glyphsJson = data.get_glyphs();

        for (int i = 0; i < glyphsJson.size(); i++) {
            GlyphInfo glyph;
            quicktype::Glyph g = glyphsJson.at(i);

            if (g.get_plane_bounds() != NULL) {
                glyph.pos = glm::vec2(g.get_plane_bounds()->get_left(), g.get_plane_bounds()->get_bottom());
                glyph.size = glm::vec2(g.get_plane_bounds()->get_right() - g.get_plane_bounds()->get_left(), g.get_plane_bounds()->get_top() - g.get_plane_bounds()->get_bottom());
            }

            if (g.get_atlas_bounds() != NULL)
                glyph.uv = glm::vec4(g.get_atlas_bounds()->get_left(), g.get_atlas_bounds()->get_bottom(), g.get_atlas_bounds()->get_right(), g.get_atlas_bounds()->get_top());

            if (g.get_advance() != NULL)
                glyph.advance = g.get_advance();

            if (g.get_unicode() != NULL)
                glyph.unicode = g.get_unicode();

            glyphs.emplace(glyph.unicode, glyph);
        }

        stbi_set_flip_vertically_on_load(1);
        atlasBuffer = stbi_load("fonts/atlas.bmp", &atlasWidth, &atlasHeight, &bpp, 4);


        i.close();

        glGenTextures(1, &atlasTextureBuffer);
        glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasBuffer);
        //glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        //glGenerateMipmap(GL_TEXTURE_2D);


        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * glyphVertices.size(), glyphVertices.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));


        //Initialize Shaders
        ShaderProgramSource source = ParseShader("res/shaders/Text.shader");
        textShader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(textShader);
        GLCall(shader_mvp = glGetUniformLocation(textShader, "u_MVP"));
        ASSERT(shader_mvp != -1);

        GLCall(shader_texturePos = glGetUniformLocation(textShader, "mainTex"));
        ASSERT(shader_texturePos != -1);

        ShaderProgramSource source1 = ParseShader("res/shaders/Basic.shader");
        debugShader = CreateShader(source1.VertexSource, source1.FragmentSource);

        glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
        glActiveTexture(GL_TEXTURE0);



        //For Bounding Box rendering:
        glGenVertexArrays(1, &bbVAO);
        glBindVertexArray(bbVAO);

        glGenBuffers(1, &bbVBO);
        glBindBuffer(GL_ARRAY_BUFFER, bbVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        //Fill with empty data
        for (int i = 0; i < 6; i++)
            bbVertices.push_back(glm::vec4(0.0f));
    }

    /* Used for adding new text to the buffer. Each glyph has its own width, height and spacing.
    * Those are appropriately calculated as per the metadata
    */
    void TextRenderer::addTextToBuffer(TextLabel* _label) {

        //TODO: Text- Bounding Box Logic and Wrapping
        //TODO: Text- Left and Right Align
        TextLabel* label = _label;

        const char* txt = label->text.c_str();

        float totalWidth = label->getTotalWidth();

        float offsetX = 0;

        float posOffsetX = 0.0f;
        float posOffsetY = 0.0f;

        if (label->pivot == Pivot::BOTTOM_LEFT) {
            posOffsetX = label->box.x0;
            posOffsetY = label->box.y1;
        }
        else if (label->pivot == Pivot::CENTER) {
            posOffsetX = label->box.x0 + (label->box.x1 - label->box.x0) / 2.0f - totalWidth / 2.0f;
            posOffsetY = label->box.y0 + (label->box.y1 - label->box.y0) / 2.0f + (label->fontSize * 0.75f) / 2.0f;
        }
        else if (label->pivot == Pivot::TOP_LEFT) {
            posOffsetX = label->box.x0;
            posOffsetY = label->box.y0 + label->fontSize * 0.75f;
        }
        else if (label->pivot == Pivot::TOP_RIGHT) {
            posOffsetX = label->box.x1 - totalWidth;
            posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize * 0.75f;
        }
        else if (label->pivot == Pivot::BOTTOM_RIGHT) {
            posOffsetX = label->box.x1 - totalWidth;
            posOffsetY = label->box.y1;
        }

        glm::vec2 pos = label->pos;

        for (int i = 0; i < label->text.length(); i++) {

            GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

            if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {

                float scale = label->fontSize;
                float originX = posOffsetX + pos.x;
                float originY = posOffsetY + pos.y;

                glyphVertices.push_back((g1.pos.x + offsetX) * scale + originX); //x
                glyphVertices.push_back(-g1.pos.y * scale + originY); //y
                glyphVertices.push_back(label->zDepth); //z
                glyphVertices.push_back(g1.uv.x / atlasInfo.size.x); // UVx
                glyphVertices.push_back(g1.uv.y / atlasInfo.size.y); //UVy

                glyphVertices.push_back((g1.pos.x + g1.size.x + offsetX) * scale + originX);//X
                glyphVertices.push_back(-(g1.pos.y + g1.size.y) * scale + originY); //Y
                glyphVertices.push_back(label->zDepth);//z
                glyphVertices.push_back(g1.uv.z / atlasInfo.size.x); //UVx
                glyphVertices.push_back(g1.uv.w / atlasInfo.size.y); //UVy

                glyphVertices.push_back(((g1.pos.x + offsetX) * scale + originX)); //x
                glyphVertices.push_back((-(g1.pos.y + g1.size.y) * scale + originY)); //y
                glyphVertices.push_back((label->zDepth));//z
                glyphVertices.push_back((g1.uv.x / atlasInfo.size.x)); //UVx
                glyphVertices.push_back((g1.uv.w / atlasInfo.size.y)); //UVy

                glyphVertices.push_back(((g1.pos.x + g1.size.x + offsetX) * scale + originX));//X
                glyphVertices.push_back((-(g1.pos.y + g1.size.y) * scale + originY)); //Y
                glyphVertices.push_back((label->zDepth));//z
                glyphVertices.push_back((g1.uv.z / atlasInfo.size.x)); //UVx
                glyphVertices.push_back((g1.uv.w / atlasInfo.size.y)); //UVy

                glyphVertices.push_back(((g1.pos.x + offsetX) * scale + originX)); //x
                glyphVertices.push_back((-g1.pos.y * scale + originY)); //y
                glyphVertices.push_back((label->zDepth));//z
                glyphVertices.push_back((g1.uv.x / atlasInfo.size.x)); // UVx
                glyphVertices.push_back((g1.uv.y / atlasInfo.size.y)); //UVy

                glyphVertices.push_back(((g1.pos.x + g1.size.x + offsetX) * scale + originX)); //x
                glyphVertices.push_back((-g1.pos.y * scale + originY)); //y
                glyphVertices.push_back((label->zDepth));//z
                glyphVertices.push_back((g1.uv.z / atlasInfo.size.x)); //UVx
                glyphVertices.push_back((g1.uv.y / atlasInfo.size.y)); //UVy

            }

            offsetX += g1.advance;
        }

        isUpdateBuffer = true;

        label->added = true;
    }

    /* The render function sets up the appropriate render states
    * and issues the draw call.
    */

    void TextRenderer::render() {

        for (int i = 0; i < labels.size(); i++) {

            if (labels.at(i)->added == false) {
                addTextToBuffer(labels.at(i));
            }
            else if (labels.at(i)->update) {
                updateLabel(labels.at(i));
            }

        }

        if (isUpdateBuffer)
            updateBuffer();

        glBindVertexArray(vao);
        glUseProgram(textShader);

        GLCall(glUniformMatrix4fv(shader_mvp, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));
        GLCall(glUniform1i(shader_texturePos, 0));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);

        int continousCount = 0;
        int startIndex = 0;
        int bufferCounter = 0;

        for (int i = 0; i < labels.size(); i++) {
            if (labels.at(i)->render) {
                continousCount += labels.at(i)->text.size();
            }
            else {
                glDrawArrays(GL_TRIANGLES, startIndex * 6, continousCount * 6);
                startIndex = bufferCounter + labels.at(i)->text.size();
                continousCount = 0;
            }

            bufferCounter += labels.at(i)->text.size();

            if (i == labels.size() - 1) {
                glDrawArrays(GL_TRIANGLES, startIndex * 6, continousCount * 6);
            }
        }

        //TODO: need to figure out right number of vertices to render
        //glDrawArrays(GL_TRIANGLES, 0, glyphVertices.size()/5);


        //Draw Glyph Rects
        if (glyphDebug) {
            glUseProgram(debugShader);

            GLCall(int mvp1 = glGetUniformLocation(textShader, "u_MVP"));
            ASSERT(mvp1 != -1);
            GLCall(glUniformMatrix4fv(mvp1, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));

            glDrawArrays(GL_LINES, 0, glyphVertices.size());
        }

        //Draw Bounding Box
        if (bbDebug) {

            glBindVertexArray(bbVAO);
            glBindBuffer(GL_ARRAY_BUFFER, bbVBO);
            glUseProgram(debugShader);

            GLCall(int mvp1 = glGetUniformLocation(debugShader, "u_MVP"));
            ASSERT(mvp1 != -1);
            GLCall(glUniformMatrix4fv(mvp1, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));

            for (int i = 0; i < labels.size(); i++) {

                BoundingBox bb = labels.at(i)->box;
                float zDepth = labels.at(i)->zDepth;

                float screenVertices[] = {
                   bb.x0, bb.y0, zDepth, 1.0f,
                   bb.x0, bb.y1, zDepth, 1.0f,
                   bb.x0, bb.y1, zDepth, 1.0f,
                   bb.x1, bb.y1, zDepth, 1.0f,
                   bb.x1, bb.y1, zDepth, 1.0f,
                   bb.x1, bb.y0, zDepth, 1.0f,
                   bb.x1, bb.y0, zDepth, 1.0f,
                   bb.x0, bb.y0, zDepth, 1.0f,
                };

                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, screenVertices, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, 8);
            }
        }
    }

    //TODO: need to combine into one function call (addText)
    //TODO: need to think about what happens when update has text which is shorter
    void TextRenderer::updateLabel(TextLabel* label) {
        int index = label->index;

        //Calculate how far off in the buffer we are, to accomodate for label's text size
        int bufferOffset = label->bufferOffset;

        //Create more room in buffer
        //First delete the existing space in the buffer
        if (label->updatedText.size() != label->text.size()) {

            glyphVertices.erase(glyphVertices.begin() + bufferOffset, glyphVertices.begin() + bufferOffset + label->text.size() * 30);

            //Remove label from labels vector
            labels.erase(labels.begin() + index);

            //Change index variable for all labels
            for (int i = index; i < labels.size(); i++) {
                labels.at(i)->index--;
                labels.at(i)->bufferOffset -= bufferOffset + label->text.size() * 30;
            }

            label->text = label->updatedText;
            //Then add on to the end
            addText(label->text, label->fontSize, label->pos, label->textAlignment, label->pivot, label->zDepth, label);
        }
        else {
            //Or else just update the values in the buffer

            //TODO: Text- Bounding Box Logic and Wrapping
           //TODO: Text- Left and Right Align

            float totalWidth = label->getTotalWidth();

            float posOffsetX = 0.0f;
            float posOffsetY = 0.0f;

            if (label->pivot == Pivot::BOTTOM_LEFT) {
                posOffsetX = label->box.x0;
                posOffsetY = label->box.y1;
            }
            else if (label->pivot == Pivot::CENTER) {
                posOffsetX = label->box.x0 + (label->box.x1 - label->box.x0) / 2.0f - totalWidth / 2.0f;
                posOffsetY = label->box.y0 + (label->box.y1 - label->box.y0) / 2.0f + (label->fontSize * 0.75f) / 2.0f;
            }
            else if (label->pivot == Pivot::TOP_LEFT) {
                posOffsetX = label->box.x0;
                posOffsetY = label->box.y0 + label->fontSize * 1.0f;
            }
            else if (label->pivot == Pivot::TOP_RIGHT) {
                posOffsetX = label->box.x1 - totalWidth;
                posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize * 1.0f;
            }
            else if (label->pivot == Pivot::BOTTOM_RIGHT) {
                posOffsetX = label->box.x1 - totalWidth;
                posOffsetY = label->box.y1;
            }

            label->text = label->updatedText;


            //TODO: Center Alignment

            switch (label->textAlignment) {
            case TextAlignment::LEFT:
                bufferTextLeftAligned(label, glm::vec2(posOffsetX, posOffsetY));
                break;
            case TextAlignment::RIGHT:
                bufferTextRightAligned(label, glm::vec2(posOffsetX, posOffsetY));
                break;
            case TextAlignment::CENTER:
                bufferTextCenterAligned(label, glm::vec2(posOffsetX, posOffsetY));
                break;
            default:
                bufferTextLeftAligned(label, glm::vec2(posOffsetX, posOffsetY));
                break;
            }


            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset, sizeof(float) * 30 * label->text.size(), &glyphVertices[bufferOffset]);
        }

        label->update = false;
    }

    void TextRenderer::bufferTextRightAligned(TextLabel* label, glm::vec2 origin) {
        int bufferOffset = label->bufferOffset;
        float offsetX = 0;
        float offsetY = 0;
        float scale = label->fontSize;
        std::string text = label->text;
        const char* txt = text.c_str();

        int lineStartCount = 0;
        int leftToUpdate = 0;

        for (int i = 0; i < text.size(); i++) {

            GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

            if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {

                float spaceNeeded = (g1.pos.x + offsetX + g1.size.x) * scale + origin.x;
                leftToUpdate++;

                if (spaceNeeded > label->box.x1) {
                    float gap = 0;

                    if (i > 0) {
                        if ((int)txt[i - 1] == 32) {
                            gap = label->box.x1 - ((offsetX - getGlyphInfo(32).advance) * scale + origin.x);
                        }
                        else {
                            gap = label->box.x1 - (offsetX * scale + origin.x);
                        }
                    }
                    //gap = 0;
                    offsetX = 0;
                    float offsetXInternal = 0;

                    for (int j = lineStartCount; j < i; j++) {

                        GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);

                        //Add first line to buffer
                        if (g2.size.x != 0 && g2.size.y != 0 ((int)txt[j]) != 32) {

                            float spaceNeededY = origin.y + offsetY;

                            //Text will overflow bottom
                            if (spaceNeededY > label->box.y1) {

                                for (int k = 0; k < 30; k++) {
                                    glyphVertices.at(j * 30 + k + bufferOffset) = 0.0f;
                                }
                            }
                            else {

                                glyphVertices.at(j * 30 + 0 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 1 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 2 + bufferOffset) = (label->zDepth); //z
                                glyphVertices.at(j * 30 + 3 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                                glyphVertices.at(j * 30 + 4 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 5 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                                glyphVertices.at(j * 30 + 6 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                                glyphVertices.at(j * 30 + 7 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 8 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 9 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 10 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 11 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 12 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 13 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 14 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 15 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                                glyphVertices.at(j * 30 + 16 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                                glyphVertices.at(j * 30 + 17 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 18 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 19 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 20 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 21 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 22 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 23 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                                glyphVertices.at(j * 30 + 24 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 25 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 26 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 27 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 28 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 29 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy
                            }

                        }

                        offsetXInternal += g2.advance;
                    }

                    lineStartCount = i;
                    offsetY += scale;
                    leftToUpdate = 0;
                }
            }

            offsetX += g1.advance;

            //Update the ones left
            if (i == text.size() - 1) {
                offsetX = 0;
                float spaceNeeded = 0;
                float gap = 0;

                for (int j = lineStartCount; j < text.size(); j++) {
                    GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);
                    offsetX += g2.advance;

                    if (j == text.size() - 1) {
                        if ((int)txt[j] == 32) {
                            offsetX -= g2.advance;
                        }
                    }
                }

                gap = label->box.x1 - (offsetX * scale + origin.x);

                float offsetXInternal = 0;

                for (int j = lineStartCount; j < text.size(); j++) {

                    GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);

                    //Add first line to buffer
                    if (g2.size.x != 0 && g2.size.y != 0 ((int)txt[j]) != 32) {

                        float spaceNeededY = origin.y + offsetY;

                        //Text will overflow bottom
                        if (spaceNeededY > label->box.y1) {

                            for (int k = 0; k < 30; k++) {
                                glyphVertices.at(j * 30 + k + bufferOffset) = 0.0f;
                            }
                        }
                        else {

                            glyphVertices.at(j * 30 + 0 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 1 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 2 + bufferOffset) = (label->zDepth); //z
                            glyphVertices.at(j * 30 + 3 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                            glyphVertices.at(j * 30 + 4 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 5 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                            glyphVertices.at(j * 30 + 6 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                            glyphVertices.at(j * 30 + 7 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 8 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 9 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 10 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 11 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 12 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 13 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 14 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 15 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                            glyphVertices.at(j * 30 + 16 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                            glyphVertices.at(j * 30 + 17 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 18 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 19 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 20 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 21 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 22 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 23 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                            glyphVertices.at(j * 30 + 24 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 25 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 26 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 27 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 28 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 29 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy
                        }

                    }

                    offsetXInternal += g2.advance;
                }
            }
        }
    }

    void TextRenderer::bufferTextCenterAligned(TextLabel* label, glm::vec2 origin) {
        int bufferOffset = label->bufferOffset;
        float offsetX = 0;
        float offsetY = 0;
        float scale = label->fontSize;
        std::string text = label->text;
        const char* txt = text.c_str();

        int lineStartCount = 0;
        int leftToUpdate = 0;

        for (int i = 0; i < text.size(); i++) {

            GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

            if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {

                float spaceNeeded = (g1.pos.x + offsetX + g1.size.x) * scale + origin.x;
                leftToUpdate++;

                if (spaceNeeded > label->box.x1) {
                    float gap = 0;

                    if (i > 0) {
                        if ((int)txt[i - 1] == 32) {
                            gap = label->box.x1 - ((offsetX - getGlyphInfo(32).advance) * scale + origin.x);
                        }
                        else {
                            gap = label->box.x1 - (offsetX * scale + origin.x);
                        }
                    }

                    gap /= 2.0f;
                    //gap = 0;
                    offsetX = 0;
                    float offsetXInternal = 0;

                    for (int j = lineStartCount; j < i; j++) {

                        GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);

                        //Add first line to buffer
                        if (g2.size.x != 0 && g2.size.y != 0 ((int)txt[j]) != 32) {

                            float spaceNeededY = origin.y + offsetY;

                            //Text will overflow bottom
                            if (spaceNeededY > label->box.y1) {

                                for (int k = 0; k < 30; k++) {
                                    glyphVertices.at(j * 30 + k + bufferOffset) = 0.0f;
                                }
                            }
                            else {

                                glyphVertices.at(j * 30 + 0 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 1 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 2 + bufferOffset) = (label->zDepth); //z
                                glyphVertices.at(j * 30 + 3 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                                glyphVertices.at(j * 30 + 4 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 5 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                                glyphVertices.at(j * 30 + 6 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                                glyphVertices.at(j * 30 + 7 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 8 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 9 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 10 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 11 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 12 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 13 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 14 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 15 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                                glyphVertices.at(j * 30 + 16 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                                glyphVertices.at(j * 30 + 17 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 18 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 19 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 20 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 21 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 22 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 23 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                                glyphVertices.at(j * 30 + 24 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                                glyphVertices.at(j * 30 + 25 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap); //x
                                glyphVertices.at(j * 30 + 26 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                                glyphVertices.at(j * 30 + 27 + bufferOffset) = (label->zDepth);//z
                                glyphVertices.at(j * 30 + 28 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                                glyphVertices.at(j * 30 + 29 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy
                            }

                        }

                        offsetXInternal += g2.advance;
                    }

                    lineStartCount = i;
                    offsetY += scale;
                    leftToUpdate = 0;
                }
            }

            offsetX += g1.advance;

            //Update the ones left
            if (i == text.size() - 1) {
                offsetX = 0;
                float spaceNeeded = 0;
                float gap = 0;

                for (int j = lineStartCount; j < text.size(); j++) {
                    GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);
                    offsetX += g2.advance;

                    if (j == text.size() - 1) {
                        if ((int)txt[j] == 32) {
                            offsetX -= g2.advance;
                        }
                    }
                }

                gap = label->box.x1 - (offsetX * scale + origin.x);

                gap /= 2.0f;

                float offsetXInternal = 0;

                for (int j = lineStartCount; j < text.size(); j++) {

                    GlyphInfo g2 = TextRenderer::GetInstance()->getGlyphInfo(txt[j]);

                    //Add first line to buffer
                    if (g2.size.x != 0 && g2.size.y != 0 ((int)txt[j]) != 32) {

                        float spaceNeededY = origin.y + offsetY;

                        //Text will overflow bottom
                        if (spaceNeededY > label->box.y1) {

                            for (int k = 0; k < 30; k++) {
                                glyphVertices.at(j * 30 + k + bufferOffset) = 0.0f;
                            }
                        }
                        else {

                            glyphVertices.at(j * 30 + 0 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 1 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 2 + bufferOffset) = (label->zDepth); //z
                            glyphVertices.at(j * 30 + 3 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                            glyphVertices.at(j * 30 + 4 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 5 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                            glyphVertices.at(j * 30 + 6 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                            glyphVertices.at(j * 30 + 7 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 8 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 9 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 10 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 11 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 12 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 13 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 14 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 15 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap);//X
                            glyphVertices.at(j * 30 + 16 + bufferOffset) = (-(g2.pos.y + g2.size.y) * scale + origin.y + offsetY); //Y
                            glyphVertices.at(j * 30 + 17 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 18 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 19 + bufferOffset) = (g2.uv.w / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 20 + bufferOffset) = ((g2.pos.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 21 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 22 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 23 + bufferOffset) = (g2.uv.x / atlasInfo.size.x); // UVx
                            glyphVertices.at(j * 30 + 24 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy

                            glyphVertices.at(j * 30 + 25 + bufferOffset) = ((g2.pos.x + g2.size.x + offsetXInternal) * scale + origin.x + gap); //x
                            glyphVertices.at(j * 30 + 26 + bufferOffset) = (-g2.pos.y * scale + origin.y + offsetY); //y
                            glyphVertices.at(j * 30 + 27 + bufferOffset) = (label->zDepth);//z
                            glyphVertices.at(j * 30 + 28 + bufferOffset) = (g2.uv.z / atlasInfo.size.x); //UVx
                            glyphVertices.at(j * 30 + 29 + bufferOffset) = (g2.uv.y / atlasInfo.size.y); //UVy
                        }

                    }

                    offsetXInternal += g2.advance;
                }
            }
        }
    }

    void TextRenderer::bufferTextLeftAligned(TextLabel* label, glm::vec2 origin) {

        int bufferOffset = label->bufferOffset;
        float offsetX = 0;
        float offsetY = 0;
        float scale = label->fontSize;
        std::string text = label->text;
        const char* txt = text.c_str();

        for (int i = 0; i < text.length(); i++) {

            GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

            if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {

                float spaceNeededX = (g1.pos.x + offsetX + g1.size.x) * scale + origin.x;

                //Text will overflow to right
                if (spaceNeededX > label->box.x1) {
                    offsetX = 0;
                    offsetY += scale;
                }

                float spaceNeededY = origin.y + offsetY;

                //Text will overflow bottom
                if (spaceNeededY > label->box.y1) {

                    for (int j = 0; j < 30; j++) {
                        glyphVertices.at(i * 30 + j + bufferOffset) = 0.0f;
                    }
                }
                else {

                    glyphVertices.at(i * 30 + 0 + bufferOffset) = ((g1.pos.x + offsetX) * scale + origin.x); //x
                    glyphVertices.at(i * 30 + 1 + bufferOffset) = (-g1.pos.y * scale + origin.y + offsetY); //y
                    glyphVertices.at(i * 30 + 2 + bufferOffset) = (label->zDepth); //z
                    glyphVertices.at(i * 30 + 3 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); // UVx
                    glyphVertices.at(i * 30 + 4 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy

                    glyphVertices.at(i * 30 + 5 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * scale + origin.x);//X
                    glyphVertices.at(i * 30 + 6 + bufferOffset) = (-(g1.pos.y + g1.size.y) * scale + origin.y + offsetY); //Y
                    glyphVertices.at(i * 30 + 7 + bufferOffset) = (label->zDepth);//z
                    glyphVertices.at(i * 30 + 8 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                    glyphVertices.at(i * 30 + 9 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                    glyphVertices.at(i * 30 + 10 + bufferOffset) = ((g1.pos.x + offsetX) * scale + origin.x); //x
                    glyphVertices.at(i * 30 + 11 + bufferOffset) = (-(g1.pos.y + g1.size.y) * scale + origin.y + offsetY); //y
                    glyphVertices.at(i * 30 + 12 + bufferOffset) = (label->zDepth);//z
                    glyphVertices.at(i * 30 + 13 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); //UVx
                    glyphVertices.at(i * 30 + 14 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                    glyphVertices.at(i * 30 + 15 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * scale + origin.x);//X
                    glyphVertices.at(i * 30 + 16 + bufferOffset) = (-(g1.pos.y + g1.size.y) * scale + origin.y + offsetY); //Y
                    glyphVertices.at(i * 30 + 17 + bufferOffset) = (label->zDepth);//z
                    glyphVertices.at(i * 30 + 18 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                    glyphVertices.at(i * 30 + 19 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                    glyphVertices.at(i * 30 + 20 + bufferOffset) = ((g1.pos.x + offsetX) * scale + origin.x); //x
                    glyphVertices.at(i * 30 + 21 + bufferOffset) = (-g1.pos.y * scale + origin.y + offsetY); //y
                    glyphVertices.at(i * 30 + 22 + bufferOffset) = (label->zDepth);//z
                    glyphVertices.at(i * 30 + 23 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); // UVx
                    glyphVertices.at(i * 30 + 24 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy

                    glyphVertices.at(i * 30 + 25 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * scale + origin.x); //x
                    glyphVertices.at(i * 30 + 26 + bufferOffset) = (-g1.pos.y * scale + origin.y + offsetY); //y
                    glyphVertices.at(i * 30 + 27 + bufferOffset) = (label->zDepth);//z
                    glyphVertices.at(i * 30 + 28 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                    glyphVertices.at(i * 30 + 29 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy
                }

            }

            offsetX += g1.advance;
        }
    }

    TextLabel* TextRenderer::addText(const std::string& text, float fontSize, glm::vec2 pos, TextAlignment alignment, Pivot pivot, float zDepth, TextLabel* _label) {

        TextLabel* label;

        if (_label == NULL) {
            label = new TextLabel(text, fontSize, pos, alignment, pivot, zDepth);
        }
        else {
            label = _label;
        }

        label->index = labels.size();
        labels.push_back(label);

        int index = label->index;

        //Calculate how far off in the buffer we are, to accomodate for label's text size
        int bufferOffset = 0;
        for (int i = 0; i < index; i++) {
            bufferOffset += labels.at(i)->text.size() * 30;
        }

        label->bufferOffset = bufferOffset;
        label->added = false;
        label->update = true;

        return label;
    }

    TextLabel* TextRenderer::addText(const std::string& _text, glm::vec2 _pos, glm::vec2 _size, float _fontSize, TextAlignment alignment, float _zDepth, TextLabel* _label) {
        TextLabel* label;

        if (_label == NULL) {
            label = new TextLabel(_text, _fontSize, _pos, _size, alignment, _zDepth);
        }
        else {
            label = _label;
        }

        label->index = labels.size();
        labels.push_back(label);

        int index = label->index;

        //Calculate how far off in the buffer we are, to accomodate for label's text size
        int bufferOffset = 0;
        for (int i = 0; i < index; i++) {
            bufferOffset += labels.at(i)->text.size() * 30;
        }

        label->bufferOffset = bufferOffset;
        label->added = false;
        label->update = true;

        return label;
    }

    void TextRenderer::removeText(TextLabel* label) {
        int index = label->index;

        //Calculate how far off in the buffer we are, to accomodate for label's text size
        int bufferOffset = 0;

        if (label->added) {
            bufferOffset = label->bufferOffset;

            glyphVertices.erase(glyphVertices.begin() + bufferOffset, glyphVertices.begin() + bufferOffset + label->text.size() * 30);
        }
        //Remove label from labels vector
        labels.erase(labels.begin() + index);

        //Change index variable for all labels
        for (int i = index; i < labels.size(); i++) {
            labels.at(i)->index--;
            labels.at(i)->bufferOffset -= bufferOffset + label->text.size() * 30;
        }

        isUpdateBuffer = true;
    }

    void TextRenderer::updateBuffer() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * glyphVertices.size(), glyphVertices.data(), GL_DYNAMIC_DRAW);

        isUpdateBuffer = false;
    }

    void TextRenderer::deinit() {
        free(atlasBuffer);
        //free(bitmap);

        glDeleteVertexArrays(1, &vao);
        glDeleteVertexArrays(1, &bbVAO);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &bbVBO);
        glDeleteProgram(textShader);
        glDeleteProgram(debugShader);

        glDeleteTextures(1, &atlasTextureBuffer);

        for (int i = 0; i < labels.size(); i++) {
            delete labels.at(i);
        }

        textRenderer_ = nullptr;
        delete this;
    }

    void TextRenderer::enableGlyphBox(bool visible) {
        glyphDebug = visible;
    }

    void TextRenderer::enableBoundingBox(bool visible) {
        bbDebug = visible;
    }


    //Helper functions to extract glyph info
    GlyphInfo TextRenderer::getGlyphInfo(char c) {

        try {
            checkGlyph(c);
            return glyphs.at((int)c);
        }
        catch (const char* msg) {
            //TODO: Implement Exception and error handling
        }

        return glyphs.at(32);
    }

    void TextRenderer::checkGlyph(char c) {
        if (glyphs.count(c) == 0) {
            throw "Loaded glyph set doesnt contain character info for: " + std::to_string(c);
        }
    }

    AtlasInfo TextRenderer::getAtlasInfo() {
        return atlasInfo;
    }
}