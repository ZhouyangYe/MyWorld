#pragma once

namespace MyWorld {
	class Event {
	public:
		const enum class TYPE : int
		{
			KEY_PRESS   = 0,
			MOUSE_MOVE  = 1,
			MOUSE_PRESS = 2,
			MOUSE_WHEEL = 3,
			WINDOW      = 4
		};
		const TYPE type;

		Event(TYPE type)
			: type(type)
		{}
	};
}
