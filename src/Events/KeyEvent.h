#pragma once
#include "Event.h"
#include "KeyCode.h"

namespace MyWorld {
	class KeyPressEvent: public Event
	{
	public:
		const enum class ACTION : int
		{
			KEY_DOWN = 0,
			KEY_UP = 1,
			KEY_HOLD = 2,
		};
		const KeyCode keyCode;
		const ACTION action;
		KeyPressEvent(Event::TYPE type, KeyCode keyCode, ACTION action)
			: Event(type), keyCode(keyCode), action(action)
		{
			
		};
	};
}
