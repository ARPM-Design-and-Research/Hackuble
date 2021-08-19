#pragma once

#include "IconRenderer.h"
#include "Icon.h"
#include "JsonParserAtlas.h"

#include <stdio.h>

using namespace SynGUI;

IconRenderer* IconRenderer::iconRenderer_ = nullptr;

IconRenderer::IconRenderer() {

}

IconRenderer* IconRenderer::GetInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (iconRenderer_ == nullptr) {
        iconRenderer_ = new IconRenderer();
    }
    return iconRenderer_;
}


/* Read the atlas metadata and store it in the appropriate structure.
* Also load the atlas Image using stbi and upload it to the gpu. The memory on the cpu
* can then be freed up after this point.
* 
* The appropriate VAO, VBO's and shaders are also intialized
*/
void IconRenderer::init() {

    std::ifstream i("icons/iconAtlas.json");
    nlohmann::json j;
    i >> j;

    quicktype::IconData data = nlohmann::json::parse(j.dump());

    atlasSize.x = data.get_meta().get_size().get_w();
    atlasSize.y = data.get_meta().get_size().get_h();

    iconMetadata = data.get_frames();

    int atlasWidth, atlasHeight;
    stbi_set_flip_vertically_on_load(1);
    atlasBuffer = stbi_load("icons/iconAtlas.png", &atlasWidth, &atlasHeight, &bpp, 4);

    i.close();

    glGenTextures(1, &atlasTextureBuffer);
    glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasBuffer);
    //glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    //glGenerateMipmap(GL_TEXTURE_2D);

    //Free texture atlas memoryfrom the cpu
    free(atlasBuffer);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * iconVertices.size(), iconVertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float)*3));


    //Initialize Shaders
    ShaderProgramSource source1 = ParseShader("res/shaders/Icon.shader");
    iconShader = CreateShader(source1.VertexSource, source1.FragmentSource);
    glUseProgram(iconShader);

    GLCall(shader_mvp = glGetUniformLocation(iconShader, "u_MVP"));
    ASSERT(shader_mvp != -1);

    GLCall(shader_atlasTexPos = glGetUniformLocation(iconShader, "mainTex"));
    ASSERT(shader_atlasTexPos != -1);

    glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
    glActiveTexture(GL_TEXTURE0);
}


/* Rendering is simply setting the appropriate OpenGL states
*  Passing on data to the shader and issuing the draw call.
* 
*  Note that to minimize draw calls, the uniforms are set once
*  and rendered with a single draw call. The vertex data contains everything
*  needed to render the icon.
* 
*  This is a tradeoff between performance and GPU memory
*/
void IconRenderer::render() {

    for (int i = 0; i < icons.size(); i++) {

        if (icons.at(i)->added == false)
            addIconToBuffer(icons.at(i));

        if(icons.at(i)->update)
            updateIcon(icons.at(i));
    }

    if (isUpdateBuffer)
        updateBuffer();

    glUseProgram(iconShader);
    glBindVertexArray(vao);

    GLCall(glUniformMatrix4fv(shader_mvp, 1, GL_FALSE, &Camera::GetInstance()->getMVP()[0][0]));

    GLCall(glUniform1i(shader_atlasTexPos, 0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTextureBuffer);

    //glDrawArrays(GL_TRIANGLES, 0, iconVertices.size()/5);

    int continousCount = 0;
    int startIndex = 0;

    for (int i = 0; i < icons.size(); i++) {
        if (icons.at(i)->render) {
            continousCount++;
        }
        else {
            glDrawArrays(GL_TRIANGLES, startIndex * 6, continousCount * 6);
            startIndex = i + 1;
            continousCount = 0;
        }

        if (i == icons.size() - 1) {
            glDrawArrays(GL_TRIANGLES, startIndex * 6, continousCount * 6);
        }
    }
}

/* Add a new icon/texture and update the iconVertices buffer
*/
void IconRenderer::addIconToBuffer(Icon* _icon) {
    Icon* icon = _icon;

    glm::vec2 p1 = glm::vec2(icon->pos.x, icon->pos.y);
    glm::vec2 p2 = glm::vec2(icon->pos.x + icon->size.x, icon->pos.y);
    glm::vec2 p3 = glm::vec2(icon->pos.x + icon->size.x, icon->pos.y + icon->size.y);
    glm::vec2 p4 = glm::vec2(icon->pos.x, icon->pos.y + icon->size.y);

    /*glm::vec2 p1 = glm::vec2(icon->box.x0, icon->box.y0);
    glm::vec2 p2 = glm::vec2(icon->box.x0, icon->box.y1);
    glm::vec2 p3 = glm::vec2(icon->box.x1, icon->box.y1);
    glm::vec2 p4 = glm::vec2(icon->box.x1, icon->box.y0);*/

    IconInfo iconInfo = getIconInfo(_icon->iconName);

    glm::vec2 t1 = glm::vec2(iconInfo.pos.x / atlasSize.x, -iconInfo.pos.y / atlasSize.y);
    glm::vec2 t2 = glm::vec2(iconInfo.pos.x / atlasSize.x, -(iconInfo.pos.y + iconInfo.size.y) / atlasSize.y);
    glm::vec2 t3 = glm::vec2((iconInfo.pos.x + iconInfo.size.x) / atlasSize.x, -(iconInfo.pos.y + iconInfo.size.y) / atlasSize.y);
    glm::vec2 t4 = glm::vec2((iconInfo.pos.x + iconInfo.size.x) / atlasSize.x, -iconInfo.pos.y / atlasSize.y);


    //TODO: Add Pivot Functionality
    iconVertices.push_back(p1.x);
    iconVertices.push_back(p1.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t1.x);
    iconVertices.push_back(t1.y);

    iconVertices.push_back(p2.x);
    iconVertices.push_back(p2.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t2.x);
    iconVertices.push_back(t2.y);

    iconVertices.push_back(p3.x);
    iconVertices.push_back(p3.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t3.x);
    iconVertices.push_back(t3.y);

    iconVertices.push_back(p1.x);
    iconVertices.push_back(p1.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t1.x);
    iconVertices.push_back(t1.y);

    iconVertices.push_back(p3.x);
    iconVertices.push_back(p3.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t3.x);
    iconVertices.push_back(t3.y);

    iconVertices.push_back(p4.x);
    iconVertices.push_back(p4.y);
    iconVertices.push_back(_icon->zDepth);
    iconVertices.push_back(t4.x);
    iconVertices.push_back(t4.y);

    isUpdateBuffer = true;

    icon->added = true;
}

/* If the icon is updated by the application, this needs to be reflected in the buffer 
* and eventually in the gpu. This function takes care of updating all the neccessary vertices
* if the update tag on the icon is set to true.
*/
void IconRenderer::updateIcon(Icon* icon) {

    int offset = icon->index * 30;

    glm::vec2 p1 = glm::vec2(icon->pos.x, icon->pos.y);
    glm::vec2 p2 = glm::vec2(icon->pos.x + icon->size.x, icon->pos.y);
    glm::vec2 p3 = glm::vec2(icon->pos.x + icon->size.x, icon->pos.y + icon->size.y);
    glm::vec2 p4 = glm::vec2(icon->pos.x, icon->pos.y + icon->size.y);

    IconInfo iconInfo = getIconInfo(icon->iconName);

    glm::vec2 t1 = glm::vec2(iconInfo.pos.x / atlasSize.x, -iconInfo.pos.y / atlasSize.y);
    glm::vec2 t2 = glm::vec2(iconInfo.pos.x / atlasSize.x, -(iconInfo.pos.y + iconInfo.size.y) / atlasSize.y);
    glm::vec2 t3 = glm::vec2((iconInfo.pos.x + iconInfo.size.x) / atlasSize.x, -(iconInfo.pos.y + iconInfo.size.y) / atlasSize.y);
    glm::vec2 t4 = glm::vec2((iconInfo.pos.x + iconInfo.size.x) / atlasSize.x, -iconInfo.pos.y / atlasSize.y);

    iconVertices[offset + 0] = p1.x;
    iconVertices[offset + 1] = p1.y;
    iconVertices[offset + 2] = icon->zDepth;
    iconVertices[offset + 3] = t1.x;
    iconVertices[offset + 4] = t1.y;

    iconVertices[offset + 5] = p2.x;
    iconVertices[offset + 6] = p2.y;
    iconVertices[offset + 7] = icon->zDepth;
    iconVertices[offset + 8] = t2.x;
    iconVertices[offset + 9] = t2.y;

    iconVertices[offset + 10] = p3.x;
    iconVertices[offset + 11] = p3.y;
    iconVertices[offset + 12] = icon->zDepth;
    iconVertices[offset + 13] = t3.x;
    iconVertices[offset + 14] = t3.y;

    iconVertices[offset + 15] = p1.x;
    iconVertices[offset + 16] = p1.y;
    iconVertices[offset + 17] = icon->zDepth;
    iconVertices[offset + 18] = t1.x;
    iconVertices[offset + 19] = t1.y;

    iconVertices[offset + 20] = p3.x;
    iconVertices[offset + 21] = p3.y;
    iconVertices[offset + 22] = icon->zDepth;
    iconVertices[offset + 23] = t3.x;
    iconVertices[offset + 24] = t3.y;

    iconVertices[offset + 25] = p4.x;
    iconVertices[offset + 26] = p4.y;
    iconVertices[offset + 27] = icon->zDepth;
    iconVertices[offset + 28] = t4.x;
    iconVertices[offset + 29] = t4.y;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * offset, sizeof(float) * 30, &iconVertices[offset]);

    icon->update = false;
}

//Cleanup
void IconRenderer::deinit() {

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(iconShader);
    glDeleteTextures(1, &atlasTextureBuffer);

    for (int i = 0; i < icons.size(); i++) {
        delete icons.at(i);
    }

    iconRenderer_ = nullptr;
    delete this;
}


/* This function is used to get the icon metadata from its string name.
*  The data is stored in a hashmap with the name as the key.
*/
IconInfo IconRenderer::getIconInfo(const std::string& iconName) {
    IconInfo info;

    try {
        checkIconSet(iconName);
        quicktype::FrameValue frameValue = iconMetadata.at(iconName);

        info.pos.x = frameValue.get_frame().get_x();
        info.pos.y = frameValue.get_frame().get_y();
        info.size.x = frameValue.get_frame().get_w();
        info.size.y = frameValue.get_frame().get_h();

        return info;
    } catch (std::out_of_range& const e) {
        std::cerr << e.what() << std::endl;
    }

    info.pos.x = 0;
    info.pos.y = 0;
    info.size.x = 0;
    info.size.y = 0;

    return info;
}

void IconRenderer::checkIconSet(const std::string& icon) {
    if (iconMetadata.count(icon) == 0) {
        throw "Loaded icon set doesn't contain icon for: " + icon;
    }
}

void IconRenderer::updateBuffer() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * iconVertices.size(), iconVertices.data(), GL_DYNAMIC_DRAW);

    isUpdateBuffer = false;
}

void IconRenderer::removeIcon(Icon* icon) {
    int index = icon->index;

    if (icon->added) {
        int bufferOffset = 0;
        for (int i = 0; i < index; i++) {
            bufferOffset += 30;
        }

        iconVertices.erase(iconVertices.begin() + bufferOffset, iconVertices.begin() + bufferOffset + 30);
    }

    icons.erase(icons.begin() + index);

    for (int i = index; i < icons.size(); i++) {
        icons.at(i)->index--;
    }

    isUpdateBuffer = true;
}

Icon* IconRenderer::addIcon(const std::string& iconName, glm::vec2 pos, glm::vec2 size, Pivot pivot, float zDepth) {
    Icon* icon = new Icon(iconName, pos, size, pivot, zDepth);
    icon->index = icons.size();
    icons.push_back(icon);

    icon->added = false;

    return icon;
}