#pragma once

namespace MyWorld {
	class Event {
	public:
		const enum TYPE : int
		{
			KEY_PRESS = 0,
			MOUSE_MOVE = 1,
			MOUSE_PRESS = 2,
			MOUSE_WHEEL = 3,
			WINDOW = 4,
		};
		const int type;

		Event(int type)
			: type(type)
		{}
	};
}
