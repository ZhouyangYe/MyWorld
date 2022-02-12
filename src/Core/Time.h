#pragma once
#include <iostream>
#include "Window/Window.h"

namespace MyWorld
{
	class Time
	{
	private:
		static double deltaTime;
		static double prevTime;
	public:
		static double getDeltaTime();
		static void updateTime();
	};
}
