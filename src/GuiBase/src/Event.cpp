#pragma once

#include "Event.h"
#include "Enum.h"
#include <algorithm> 
#include "Camera.h"

using namespace SynGUI;

Event::Event(EventType type) {
	eventType = type;
}

MouseEvent::MouseEvent(EventType type, float _mouseX, float _mouseY, MouseButton _button, float _deltaMouseX, float _deltaMouseY) : Event(type) {
	rawPos = glm::vec2(_mouseX, _mouseY);
	rawDelta = glm::vec2(_deltaMouseX, _deltaMouseY);
	pos = glm::vec2(0);
	delta = glm::vec2(0);

	button = _button;
	
	wheelDelta = 0;
}

MouseEvent::MouseEvent(EventType type, int _wheelDelta) : Event(type) {
	wheelDelta = _wheelDelta;

	rawPos = glm::vec2(0);
	rawDelta = glm::vec2(0);
	pos = glm::vec2(0);
	delta = glm::vec2(0);

	button = MouseButton::NONE;
}


EventHandler::EventHandler() {

}

void EventHandler::OnMouseDown(MouseEvent* eventArgs) {}
void EventHandler::OnMouseUp(MouseEvent* eventArgs) {}
void EventHandler::OnMouseMove(MouseEvent* eventArgs) {}
void EventHandler::OnMouseWheel(MouseEvent* eventArgs) {}


EventManager::EventManager() {

}

static void SynGUI::convertMouseEvent(MouseEvent* eve) {

	glm::vec4 mousePosGlobal = glm::inverse(Camera::GetInstance()->getViewMatrix()) * glm::vec4(((MouseEvent*)eve)->rawPos.x * 2.0f, ((MouseEvent*)eve)->rawPos.y * 2.0f, 0.0f, 1.0f);
	((MouseEvent*)eve)->pos.x = mousePosGlobal.x;
	((MouseEvent*)eve)->pos.y = mousePosGlobal.y;
}


void EventManager::pollEvents() {

	if (events.size() > 0) {
		for (Event* eve : events) {

			switch (eve->eventType)
			{
			case EventType::MOUSEDOWN:

				if (((MouseEvent*)eve)->button == MouseButton::LEFT)
					leftMouseDown = true;

				if (((MouseEvent*)eve)->button == MouseButton::RIGHT)
					rightMouseDown = true;

				convertMouseEvent((MouseEvent*)eve);

				break;

			case EventType::MOUSEUP:

				if (((MouseEvent*)eve)->button == MouseButton::LEFT)
					leftMouseDown = false;

				if (((MouseEvent*)eve)->button == MouseButton::RIGHT)
					rightMouseDown = false;

				convertMouseEvent((MouseEvent*)eve);

				break;

			case EventType::MOUSEMOVE:

				glm::vec4 mousePosGlobal = glm::inverse(Camera::GetInstance()->getZoomMatrix())* glm::vec4(((MouseEvent*)eve)->rawPos.x * 2.0f, ((MouseEvent*)eve)->rawPos.y * 2.0f, 0.0f, 1.0f);

				mouseDelta = glm::vec2(mousePosGlobal.x, mousePosGlobal.y) - mousePos;
				mousePos = glm::vec2(mousePosGlobal.x, mousePosGlobal.y);
				((MouseEvent*)eve)->delta = mouseDelta;
				((MouseEvent*)eve)->pos = mousePos;


				mouseRawDelta = glm::vec2(((MouseEvent*)eve)->rawPos.x, ((MouseEvent*)eve)->rawPos.y) - mouseRawPos;
				mouseRawPos = glm::vec2(((MouseEvent*)eve)->rawPos.x, ((MouseEvent*)eve)->rawPos.y);
				((MouseEvent*)eve)->rawDelta = mouseRawDelta;

				break;
			}


			for (int i = 0; i < listeners.size(); i++) {
				EventHandler* obj = listeners.at(i);
				switch (eve->eventType) {
				case EventType::MOUSEDOWN:
					obj->OnMouseDown((MouseEvent*)eve);
					break;
				case EventType::MOUSEUP:
					obj->OnMouseUp((MouseEvent*)eve);
					break;
				case EventType::MOUSEMOVE:
					obj->OnMouseMove((MouseEvent*)eve);
					break;
				case EventType::MOUSEWHEEL:
					obj->OnMouseWheel((MouseEvent*)eve);
					break;
				}
			}
		}

		for (int i = 0; i < events.size(); i++) {
			delete events.at(i);
		}

		events.clear();
	}
}

void EventManager::addEvent(Event* eve) {
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

	for (int i = 0; i < events.size(); i++) {
		delete events.at(i);
	}

	events.clear();

	for (int i = 0; i < listeners.size(); i++) {
		delete listeners.at(i);
	}

	listeners.clear();
}