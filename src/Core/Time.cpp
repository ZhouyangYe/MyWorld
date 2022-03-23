#include "Time.h"

namespace MyWorld
{
	double Time::deltaTime = 0;
	double Time::prevTime = 0;
	double Time::fpsDeltaTime = 0;
	double Time::fpsPrevTime = 0;
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
		double currentTime = Window::getCurrentTime();
		fpsDeltaTime = currentTime - fpsPrevTime;
		if (fpsDeltaTime >= 1.0f || fps == 0)
		{
			fpsPrevTime = currentTime;
			fps = round(1 / deltaTime);
		}
	}

	const int& Time::getFps()
	{
		return fps;
	}
}
