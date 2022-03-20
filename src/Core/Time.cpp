#include "Time.h"

namespace MyWorld
{
	double Time::deltaTime = 0;
	double Time::prevTime = 0;
	int Time::fps = 0;

	const double& Time::getDeltaTime()
	{
		return deltaTime;
	}

	const double Time::getCurrentTime()
	{
		return Window::getCurrentTime();
	}

	void Time::updateTime()
	{
		double currentTime = Window::getCurrentTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
	}

	void Time::updateFps()
	{
		fps = round(1 / deltaTime);
	}

	const int& Time::getFps()
	{
		return fps;
	}
}
