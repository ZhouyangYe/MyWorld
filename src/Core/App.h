#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"
#include "EventsHandler.h"
#include "World.h"
#include "Time.h"
#include "Info.h"

namespace MyWorld
{
	class App
	{
	public:
		App();
		static void Init();
		static void Terminate();
		static void Update();
	};
}
