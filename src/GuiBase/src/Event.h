#pragma once

#include <vector>
#include "Enum.h"
#include "Component.h"

#include "glm.hpp"

namespace SynGUI {

	class Event {
	public:
		Event(EventType type);
		EventType eventType;
	};

	class MouseEvent : public Event {
	public:
		MouseEvent(EventType type, float mouseX, float mouseY, MouseButton button, float _deltaMouseX = 0, float _deltaMouseY = 0);
		MouseEvent(EventType type, int wheelDelta);

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

		virtual void OnMouseDown(MouseEvent* eventArgs);
		virtual void OnMouseUp(MouseEvent* eventArgs);
		virtual void OnMouseMove(MouseEvent* eventArgs);
		virtual void OnMouseWheel(MouseEvent* eventArgs);
	};


	class EventManager {
	private:
		std::vector<EventHandler*> listeners;
		std::vector<Event*> events;

	public:
		EventManager();
		~EventManager();
		void pollEvents();
		void addEvent(Event* eve);
		void addListener(EventHandler* listener);
		void removeListener(EventHandler* listener);

		bool leftMouseDown = false;
		bool rightMouseDown = false;

		glm::vec2 mousePos;
		glm::vec2 mouseDelta;

		glm::vec2 mouseRawPos;
		glm::vec2 mouseRawDelta;
	};

	static void convertMouseEvent(MouseEvent* eventArgs);
}
