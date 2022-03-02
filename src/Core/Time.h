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
		static int fps;
	public:
		static const double& getDeltaTime();
		static void updateTime();
		static const int& getFps();
	};
}
