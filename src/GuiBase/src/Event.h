#pragma once

#include <vector>
#include "Enum.h"

#include "glm.hpp"
#include <memory>

namespace SynGUI {

	class Event {
	public:
		Event(EventType type);
		EventType eventType;
	};

	class ResizeEvent : public Event {
	public:
		ResizeEvent(EventType type, int newWidth, int newHeight);

		int width;
		int height;
	};

	class MouseEvent : public Event {
	public:
		MouseEvent(EventType type, float mouseX, float mouseY, MouseButton button, float _deltaMouseX = 0, float _deltaMouseY = 0);
		MouseEvent(EventType type, float mouseX, float mouseY, int wheelDelta);

		glm::vec2 rawPos;
		glm::vec2 rawDelta;

		glm::vec2 pos;
		glm::vec2 delta;

		int wheelDelta;

		MouseButton button;
	};

	class EventHandler {
	public:
		EventHandler();

		virtual void OnMouseDown(std::shared_ptr<MouseEvent> eventArgs);
		virtual void OnMouseUp(std::shared_ptr<MouseEvent> eventArgs);
		virtual void OnMouseMove(std::shared_ptr<MouseEvent> eventArgs);
		virtual void OnMouseWheel(std::shared_ptr<MouseEvent> eventArgs);
		virtual void OnResize(std::shared_ptr<ResizeEvent> eventArgs);
	};


	class EventManager {
	private:
		std::vector<EventHandler*> listeners;
		std::vector<std::shared_ptr<Event>> events;

	public:
		EventManager();
		~EventManager();
		void pollEvents();
		void addEvent(std::shared_ptr<Event> eve);
		void addListener(EventHandler* listener);
		void removeListener(EventHandler* listener);

		bool leftMouseDown = false;
		bool rightMouseDown = false;

		glm::vec2 mousePos;
		glm::vec2 mouseDelta;

		glm::vec2 mouseRawPos;
		glm::vec2 mouseRawDelta;
	};

	static void convertMouseEvent(std::shared_ptr<MouseEvent> eve);
}
