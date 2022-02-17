#include "Time.h"

namespace MyWorld
{
	double Time::deltaTime = 0;
	double Time::prevTime = 0;

	double Time::getDeltaTime()
	{
		return deltaTime;
	}

	void Time::updateTime()
	{
		double currentTime = Window::getCurrentTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
	}
}
