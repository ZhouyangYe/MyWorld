#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Chunk.h"
#include "Window/Window.h"
#include "EventsHandler.h"
#include "Time.h";

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
