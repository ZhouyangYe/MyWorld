#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"
#include "EventsHandler.h"
#include "Time.h"
#include "Data.h"

namespace MyWorld
{
	class App
	{
	public:
		App();
		static void Init();
		static void Terminate();
		static void Run();
	};
}
