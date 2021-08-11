#pragma once

#include "TextRenderer.h"
#include "TextLabel.h"

#include "nlohmann/json.hpp"
#include "JsonParserAtlas.h"

#include <stdio.h>

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
            glyph.size = glm::vec2(g.get_plane_bounds()->get_right() - g.get_plane_bounds()->get_left(), g.get_plane_bounds()->get_top()-g.get_plane_bounds()->get_bottom());
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*5*glyphVertices.size(), glyphVertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (GLvoid*)(sizeof(float)*3));


    //Initialize Shaders
    ShaderProgramSource source = ParseShader("res/shaders/Text.shader");
    textShader = CreateShader(source.VertexSource, source.FragmentSource);

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
        posOffsetY = label->box.y0 + (label->box.y1 - label->box.y0) / 2.0f + atlasInfo.fontSize * label->fontSize / 2.0f;
    }
    else if (label->pivot == Pivot::TOP_LEFT) {
        posOffsetX = label->box.x0;
        posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize;
    }
    else if (label->pivot == Pivot::TOP_RIGHT) {
        posOffsetX = label->box.x1 - totalWidth;
        posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize;
    }
    else if (label->pivot == Pivot::BOTTOM_RIGHT) {
        posOffsetX = label->box.x1 - totalWidth;
        posOffsetY = label->box.y1;
    }

    for (int i = 0; i < label->text.length(); i++) {

        GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

        if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {
          
            glyphVertices.push_back((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + label->pos.x); //x
            glyphVertices.push_back(-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //y
            glyphVertices.push_back(label->zDepth); //z
            glyphVertices.push_back(g1.uv.x / atlasInfo.size.x); // UVx
            glyphVertices.push_back(g1.uv.y / atlasInfo.size.y); //UVy

            glyphVertices.push_back((g1.pos.x + g1.size.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + label->pos.x);//X
            glyphVertices.push_back(-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //Y
            glyphVertices.push_back(label->zDepth);//z
            glyphVertices.push_back(g1.uv.z / atlasInfo.size.x); //UVx
            glyphVertices.push_back(g1.uv.w / atlasInfo.size.y); //UVy

            glyphVertices.push_back((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + label->pos.x); //x
            glyphVertices.push_back(-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //y
            glyphVertices.push_back(label->zDepth);//z
            glyphVertices.push_back(g1.uv.x / atlasInfo.size.x); //UVx
            glyphVertices.push_back(g1.uv.w / atlasInfo.size.y); //UVy
    


            glyphVertices.push_back((g1.pos.x + g1.size.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + label->pos.x);//X
            glyphVertices.push_back(-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //Y
            glyphVertices.push_back(label->zDepth);//z
            glyphVertices.push_back(g1.uv.z / atlasInfo.size.x); //UVx
            glyphVertices.push_back(g1.uv.w / atlasInfo.size.y); //UVy

            glyphVertices.push_back((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + label->pos.x); //x
            glyphVertices.push_back(-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //y
            glyphVertices.push_back(label->zDepth);//z
            glyphVertices.push_back(g1.uv.x / atlasInfo.size.x); // UVx
            glyphVertices.push_back(g1.uv.y / atlasInfo.size.y); //UVy

            glyphVertices.push_back((g1.pos.x + g1.size.x + offsetX)* atlasInfo.fontSize* label->fontSize + posOffsetX + label->pos.x); //x
            glyphVertices.push_back(-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + label->pos.y); //y
            glyphVertices.push_back(label->zDepth);//z
            glyphVertices.push_back(g1.uv.z / atlasInfo.size.x); //UVx
            glyphVertices.push_back(g1.uv.y / atlasInfo.size.y); //UVy

        }

        offsetX += g1.advance;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* glyphVertices.size(), glyphVertices.data(), GL_DYNAMIC_DRAW);

    label->added = true;
}

/* The render function sets up the appropriate render states
* and issues the draw call.
*/

void TextRenderer::render() {

    for (int i = 0; i < labels.size(); i++) {

        if (labels.at(i)->added == false)
            addTextToBuffer(labels.at(i));

        if (labels.at(i)->update)
            updateLabel(labels.at(i));
    }

    glBindVertexArray(vao);

    glUseProgram(textShader);

    GLCall(int mvp = glGetUniformLocation(textShader, "u_MVP"));
    ASSERT(mvp != -1);
    GLCall(glUniformMatrix4fv(mvp, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));

    GLCall(int tex = glGetUniformLocation(textShader, "mainTex"));
    ASSERT(tex != -1);
    GLCall(glUniform1i(tex, 0));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);

    //TODO: need to figure out right number of vertices to render
    glDrawArrays(GL_TRIANGLES, 0, glyphVertices.size()/5);


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
    int bufferOffset = 0;
    for (int i = 0; i < index; i++) {
        bufferOffset += labels.at(i)->text.size()*30;
    }

    //Create more room in buffer
    //First delete the existing space in the buffer
    if (label->updatedText.size() != label->text.size()) {

        glyphVertices.erase(glyphVertices.begin() + bufferOffset, glyphVertices.begin() + bufferOffset + label->text.size() * 30);

        //Remove label from labels vector
        labels.erase(labels.begin() + index);

        //Change index variable for all labels
        for (int i = index; i < labels.size(); i++) {
            labels.at(i)->index--;
        }

        label->text = label->updatedText;
        //Then add on to the end
        addText(label->text, label->fontSize, label->pos, label->pivot, label->zDepth, label);
    }
    else {
    //Or else just update the values in the buffer

    //TODO: Text- Bounding Box Logic and Wrapping
   //TODO: Text- Left and Right Align
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
            posOffsetY = label->box.y0 + (label->box.y1 - label->box.y0) / 2.0f + atlasInfo.fontSize * label->fontSize / 2.0f;
        }
        else if (label->pivot == Pivot::TOP_LEFT) {
            posOffsetX = label->box.x0;
            posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize;
        }
        else if (label->pivot == Pivot::TOP_RIGHT) {
            posOffsetX = label->box.x1 - totalWidth;
            posOffsetY = label->box.y0 + atlasInfo.fontSize * label->fontSize;
        }
        else if (label->pivot == Pivot::BOTTOM_RIGHT) {
            posOffsetX = label->box.x1 - totalWidth;
            posOffsetY = label->box.y1;
        }

        glm::vec2 pos = label->pos;

        label->text = label->updatedText;

        for (int i = 0; i < label->text.length(); i++) {

            GlyphInfo g1 = TextRenderer::GetInstance()->getGlyphInfo(txt[i]);

            if (g1.size.x != 0 && g1.size.y != 0 ((int)txt[i]) != 32) {

                glyphVertices.at(i * 30 + 0 + bufferOffset) = ((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x); //x
                glyphVertices.at(i * 30 + 1 + bufferOffset) = (-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //y
                glyphVertices.at(i * 30 + 2 + bufferOffset) = (label->zDepth); //z
                glyphVertices.at(i * 30 + 3 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); // UVx
                glyphVertices.at(i * 30 + 4 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy

                glyphVertices.at(i * 30 + 5 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x);//X
                glyphVertices.at(i * 30 + 6 + bufferOffset) = (-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //Y
                glyphVertices.at(i * 30 + 7 + bufferOffset) = (label->zDepth);//z
                glyphVertices.at(i * 30 + 8 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                glyphVertices.at(i * 30 + 9 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                glyphVertices.at(i * 30 + 10 + bufferOffset) = ((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x); //x
                glyphVertices.at(i * 30 + 11 + bufferOffset) = (-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //y
                glyphVertices.at(i * 30 + 12 + bufferOffset) = (label->zDepth);//z
                glyphVertices.at(i * 30 + 13 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); //UVx
                glyphVertices.at(i * 30 + 14 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                glyphVertices.at(i * 30 + 15 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x);//X
                glyphVertices.at(i * 30 + 16 + bufferOffset) = (-(g1.pos.y + g1.size.y) * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //Y
                glyphVertices.at(i * 30 + 17 + bufferOffset) = (label->zDepth);//z
                glyphVertices.at(i * 30 + 18 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                glyphVertices.at(i * 30 + 19 + bufferOffset) = (g1.uv.w / atlasInfo.size.y); //UVy

                glyphVertices.at(i * 30 + 20 + bufferOffset) = ((g1.pos.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x); //x
                glyphVertices.at(i * 30 + 21 + bufferOffset) = (-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //y
                glyphVertices.at(i * 30 + 22 + bufferOffset) = (label->zDepth);//z
                glyphVertices.at(i * 30 + 23 + bufferOffset) = (g1.uv.x / atlasInfo.size.x); // UVx
                glyphVertices.at(i * 30 + 24 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy

                glyphVertices.at(i * 30 + 25 + bufferOffset) = ((g1.pos.x + g1.size.x + offsetX) * atlasInfo.fontSize * label->fontSize + posOffsetX + pos.x); //x
                glyphVertices.at(i * 30 + 26 + bufferOffset) = (-g1.pos.y * atlasInfo.fontSize * label->fontSize + posOffsetY + pos.y); //y
                glyphVertices.at(i * 30 + 27 + bufferOffset) = (label->zDepth);//z
                glyphVertices.at(i * 30 + 28 + bufferOffset) = (g1.uv.z / atlasInfo.size.x); //UVx
                glyphVertices.at(i * 30 + 29 + bufferOffset) = (g1.uv.y / atlasInfo.size.y); //UVy

            }

            offsetX += g1.advance;
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset, sizeof(float) * 30 * label->text.size(), &glyphVertices[bufferOffset]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * glyphVertices.size(), glyphVertices.data(), GL_DYNAMIC_DRAW);
    }

    label->update = false;
}

TextLabel* TextRenderer::addText(const std::string& text, float fontSize, glm::vec2 pos, Pivot pivot, float zDepth, TextLabel* _label) {

    TextLabel* label;

    if (_label == NULL) {
        label = new TextLabel(text, fontSize, pos, pivot, zDepth);
        label->index = labels.size();
        labels.push_back(label);
    }
    else {
        label = _label;
        label->index = labels.size();
        labels.push_back(label);
    }

    label->added = false;

    return label;
}

void TextRenderer::deinit() {
    free(atlasBuffer);
    //free(bitmap);

    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &bbVAO);
    glDeleteBuffers(1,&vbo);
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

void TextRenderer::enableGlyphBox() {
    glyphDebug = true;
}

void TextRenderer::enableBoundingBox() {
    bbDebug = true;
}


//Helper functions to extract glyph info
GlyphInfo TextRenderer::getGlyphInfo(char c) {
    return glyphs.at((int)c);
}

AtlasInfo TextRenderer::getAtlasInfo() {
    return atlasInfo;
}