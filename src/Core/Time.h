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
		static void updateFps();
		static const int& getFps();
		static const double getCurrentTime();
	};
}
