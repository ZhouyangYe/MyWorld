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

	void Time::updateTime()
	{
		double currentTime = Window::getCurrentTime();
		deltaTime = currentTime - prevTime;
		fps = (int)(1 / deltaTime);
		prevTime = currentTime;
	}

	const int& Time::getFps()
	{
		return fps;
	}
}
