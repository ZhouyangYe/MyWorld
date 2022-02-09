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
		const KeyCode m_keyCode;
		const int m_action;
		KeyPressEvent(int type, KeyCode keyCode, int action)
			: Event(type), m_keyCode(keyCode), m_action(action)
		{
			
		};
	};
}
