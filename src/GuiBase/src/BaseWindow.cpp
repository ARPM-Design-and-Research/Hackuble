#pragma once

#include "BaseWindow.h"

using namespace SynGUI;

BaseWindow::BaseWindow(const std::string& _title, const std::string& _icon, glm::vec2 _size, glm::vec2 _pos, glm::vec3 _color, float _layer) {
	pos = _pos;
	size = _size;
	color = _color;
	title = _title;
	layer = _layer;

	//Some offsets for rendering the background rectangle and accent rectangle
	backgroundRect = RectangleRenderer::GetInstance()->addRectangle(pos, size, 3.0f, 3.0f, 3.0f, 3.0f, glm::vec3(115.0f / 255.0f), layer + 0.0f);
	topRect = RectangleRenderer::GetInstance()->addRectangle(pos, glm::vec2(size.x, 4.0f), 3.0f, 3.0f, 0.0f, 0.0f, color, layer + 0.0001f);

	//The title of the component
	titleLabel = TextRenderer::GetInstance()->addText(title, 0.15f, glm::vec2(0.0f), Pivot::CENTER);
	titleLabel->setBox(glm::vec2(pos.x, pos.y + 5.0f), glm::vec2(pos.x + size.x, pos.y + 5.0f + 14.0f));
	titleLabel->setZDepth(layer + 0.0001f);

	//Setting up the icon
	icon = IconRenderer::GetInstance()->addIcon(_icon, glm::vec2(pos.x + 3.0f, pos.y + 7.0f), glm::vec2(10.0f), Pivot::TOP_LEFT, layer + 0.0001f);

	//Tracker to keep the current height of the component, so more elements can be added dynamically
	currentHeight = 24.0f;

	calculateBoundingBox();
}

void BaseWindow::OnMouseMove(std::shared_ptr<MouseEvent> eventArgs) {
	
	if(eventArgs->button == MouseButton::LEFT)
		translate(eventArgs->delta);
}

void BaseWindow::translate(glm::vec2 _trans) {

	glm::vec2 translate = _trans;

	Component::translate(translate);

	backgroundRect->translate(translate);
	topRect->translate(translate);
	titleLabel->translate(translate);

	icon->translate(translate);

	calculateBoundingBox();
}


//Sets the positions of the component and children components
void BaseWindow::setPosition(glm::vec2 _pos) {

	glm::vec2 translate = _pos - pos;

	Component::translate(translate);
	
	backgroundRect->translate(translate);
	topRect->translate(translate);
	titleLabel->translate(translate);

	icon->translate(translate);

	calculateBoundingBox();
}

void BaseWindow::setTitle(std::string title) {
	titleLabel->updateText(title);
}

std::string BaseWindow::getTitle() {
	return titleLabel->getCurrentText();
}

void BaseWindow::setColor(glm::vec3 _color) {
	color = _color;
	topRect->setColor(color);
}

//Adds a slider to the component
Slider* BaseWindow::addSlider(const std::string& title, float currentValue, float startValue, float endValue) {
	Slider* slider = new Slider(title, pos + glm::vec2(0.0f, currentHeight), glm::vec2(size.x - 5.0f, 12.0f), currentValue, startValue, endValue, layer);
	addChildComponent(slider);

	currentHeight += 14.0f;

	backgroundRect->setSize(glm::vec2(size.x, currentHeight));

	return slider;
}

void BaseWindow::calculateBoundingBox() {
	componentBoundingBox.x0 = backgroundRect->box.x0;
	componentBoundingBox.y0 = backgroundRect->box.y0;
	componentBoundingBox.x1 = backgroundRect->box.x1;
	componentBoundingBox.y1 = backgroundRect->box.y1;
}

BaseWindow::~BaseWindow() {
	
}


/*BaseWindow* createWindow(char* title, char* icon, glm::vec2 size, glm::vec2 pos, glm::vec3 color, float layer) {

	return new BaseWindow(std::string(title), std::string(icon), size, pos, color, layer);
}


void destroyWindow(BaseWindow* ptr) {

	if (ptr != nullptr) {
		delete ptr;
	}
}


void addSlider(BaseWindow* ptr, char* title, float currentValue, float startValue, float endValue) {
	ptr->addSlider(std::string(title), currentValue, startValue, endValue);
}*/