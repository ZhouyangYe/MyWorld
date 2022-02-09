#pragma once
#include "Event.h"
#include "MouseCode.h"

namespace MyWorld {
	class MousePressEvent: public Event
	{
	public:
		const enum ACTION : int
		{
			MOUSE_DOWN = 0,
			MOUSE_UP = 1,
			MOUSE_HOLD = 2,
		};
		const MouseCode m_mouseCode;
		const ACTION m_action;
		MousePressEvent(TYPE type, MouseCode mouseCode, ACTION action)
			: Event(type), m_mouseCode(mouseCode), m_action(action)
		{

		};
	};

	class MouseWheelEvent : public Event
	{
	public:
		const float m_xOffset, m_yOffset;
		MouseWheelEvent(TYPE type, float xOffset, float yOffset)
			: Event(type), m_xOffset(xOffset), m_yOffset(yOffset)
		{

		};
	};

	class MouseMoveEvent : public Event
	{
	public:
		const float m_xPos;
		const float m_yPos;
		MouseMoveEvent(TYPE type, const float xPos, const float yPos)
			: Event(type), m_xPos(xPos), m_yPos(yPos)
		{

		};
	};
}

