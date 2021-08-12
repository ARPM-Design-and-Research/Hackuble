#pragma once

#include "Event.h"
#include "Enum.h"
#include <algorithm> 
#include "Camera.h"
#include "Component.h"

using namespace SynGUI;

Event::Event(EventType type) {
	eventType = type;
}

ResizeEvent::ResizeEvent(EventType type, int newWidth, int newHeight) : Event(type) {
	width = newWidth;
	height = newHeight;
}

MouseEvent::MouseEvent(EventType type, float _mouseX, float _mouseY, MouseButton _button, float _deltaMouseX, float _deltaMouseY) : Event(type) {
	rawPos = glm::vec2(_mouseX, _mouseY);
	rawDelta = glm::vec2(_deltaMouseX, _deltaMouseY);
	pos = glm::vec2(0);
	delta = glm::vec2(0);

	button = _button;
	
	wheelDelta = 0;
}

MouseEvent::MouseEvent(EventType type, float _mouseX, float _mouseY, int _wheelDelta) : Event(type) {
	wheelDelta = _wheelDelta;

	rawPos = glm::vec2(_mouseX, _mouseY);
	rawDelta = glm::vec2(0);
	pos = glm::vec2(0);
	delta = glm::vec2(0);

	button = MouseButton::NONE;
}


EventHandler::EventHandler() {

}

void EventHandler::OnMouseDown(std::shared_ptr<MouseEvent> eventArgs) {}
void EventHandler::OnMouseUp(std::shared_ptr<MouseEvent> eventArgs) {}
void EventHandler::OnMouseMove(std::shared_ptr<MouseEvent> eventArgs) {}
void EventHandler::OnMouseWheel(std::shared_ptr<MouseEvent> eventArgs) {}
void EventHandler::OnResize(std::shared_ptr<ResizeEvent> eventArgs) {}


EventManager::EventManager() {

}

static void SynGUI::convertMouseEvent(std::shared_ptr<MouseEvent> eve) {

	glm::vec4 mousePosGlobal = glm::inverse(Camera::GetInstance()->getViewMatrix()) * glm::vec4(eve->rawPos.x * 2.0f, eve->rawPos.y * 2.0f, 0.0f, 1.0f);
	eve->pos.x = mousePosGlobal.x;
	eve->pos.y = mousePosGlobal.y;
}


void EventManager::pollEvents() {

	if (events.size() > 0) {
		for (int i = 0; i < events.size(); i++) {
			std::shared_ptr<Event> eve = events.at(i);
			switch (eve->eventType)
			{
			case EventType::MOUSEDOWN:

				if (std::static_pointer_cast<MouseEvent>(eve)->button == MouseButton::LEFT)
					leftMouseDown = true;

				if (std::static_pointer_cast<MouseEvent>(eve)->button == MouseButton::RIGHT)
					rightMouseDown = true;

				convertMouseEvent(std::static_pointer_cast<MouseEvent>(eve));

				break;

			case EventType::MOUSEUP:

				if (std::static_pointer_cast<MouseEvent>(eve)->button == MouseButton::LEFT)
					leftMouseDown = false;

				if (std::static_pointer_cast<MouseEvent>(eve)->button == MouseButton::RIGHT)
					rightMouseDown = false;

				convertMouseEvent(std::static_pointer_cast<MouseEvent>(eve));

				break;

			case EventType::MOUSEWHEEL:

				convertMouseEvent(std::static_pointer_cast<MouseEvent>(eve));

				break;

			case EventType::MOUSEMOVE:

				std::shared_ptr<MouseEvent> mouseEvent = std::static_pointer_cast<MouseEvent>(eve);

				glm::vec4 mousePosGlobal = glm::inverse(Camera::GetInstance()->getViewMatrix()) * glm::vec4(mouseEvent->rawPos.x*2.0f, mouseEvent->rawPos.y*2.0f, 0.0f,1.0f);

				mouseDelta = glm::vec2(mousePosGlobal.x, mousePosGlobal.y) - mousePos;
				mousePos = glm::vec2(mousePosGlobal.x, mousePosGlobal.y);
				mouseEvent->delta = mouseDelta;
				mouseEvent->pos = mousePos;


				mouseRawDelta = glm::vec2(mouseEvent->rawPos.x, mouseEvent->rawPos.y) - mouseRawPos;
				mouseRawPos = glm::vec2(mouseEvent->rawPos.x, mouseEvent->rawPos.y);
				mouseEvent->rawDelta = mouseRawDelta;

				break;
			}


			for (int i = 0; i < listeners.size(); i++) {
				EventHandler* obj = listeners.at(i);
				switch (eve->eventType) {
				case EventType::MOUSEDOWN:
					obj->OnMouseDown(std::static_pointer_cast<MouseEvent>(eve));
					break;
				case EventType::MOUSEUP:
					obj->OnMouseUp(std::static_pointer_cast<MouseEvent>(eve));
					break;
				case EventType::MOUSEMOVE:
					obj->OnMouseMove(std::static_pointer_cast<MouseEvent>(eve));
					break;
				case EventType::MOUSEWHEEL:
					obj->OnMouseWheel(std::static_pointer_cast<MouseEvent>(eve));
					break;
				case EventType::RESIZE:
					obj->OnResize(std::static_pointer_cast<ResizeEvent>(eve));
					break;
				}
			}
		}

		/*for (int i = 0; i < events.size(); i++) {
			delete events.at(i);
		}*/

		events.clear();
	}
}

void EventManager::addEvent(std::shared_ptr<Event> eve) {
	events.push_back(eve);
}

void EventManager::addListener(EventHandler* listener) {
	listeners.push_back(listener);
}

void EventManager::removeListener(EventHandler* listener) {
	auto it = find(listeners.begin(), listeners.end(), listener);

	if(it != listeners.end())
		listeners.erase(it);
}

EventManager::~EventManager() {

	/*for (int i = 0; i < events.size(); i++) {
		delete events.at(i);
	}*/

	events.clear();

	for (int i = 0; i < listeners.size(); i++) {
		delete listeners.at(i);
	}

	listeners.clear();
}