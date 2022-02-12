#pragma once
#include "Event.h"
#include "KeyCode.h"

namespace MyWorld {
	class KeyPressEvent: public Event
	{
	public:
		const enum ACTION : int
		{
			KEY_DOWN = 0,
			KEY_UP = 1,
			KEY_HOLD = 2,
		};
		const KeyCode keyCode;
		const int action;
		KeyPressEvent(int type, KeyCode keyCode, int action)
			: Event(type), keyCode(keyCode), action(action)
		{
			
		};
	};
}
