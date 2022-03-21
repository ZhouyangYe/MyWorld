#pragma once
#include "Event.h"
#include "MouseCode.h"

namespace MyWorld {
	class MousePressEvent: public Event
	{
	public:
		const enum class ACTION : int
		{
			MOUSE_DOWN = 0,
			MOUSE_UP = 1,
			MOUSE_HOLD = 2,
		};
		const MouseCode mouseCode;
		const ACTION action;
		MousePressEvent(Event::TYPE type, MouseCode mouseCode, ACTION action)
			: Event(type), mouseCode(mouseCode), action(action)
		{};
	};

	class MouseWheelEvent : public Event
	{
	public:
		const float xOffset, yOffset;
		MouseWheelEvent(Event::TYPE type, float xOffset, float yOffset)
			: Event(type), xOffset(xOffset), yOffset(yOffset)
		{};
	};

	class MouseMoveEvent : public Event
	{
	public:
		const float xPos;
		const float yPos;
		MouseMoveEvent(Event::TYPE type, const float xPos, const float yPos)
			: Event(type), xPos(xPos), yPos(yPos)
		{};
	};
}

