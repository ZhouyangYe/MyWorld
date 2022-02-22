#include "App.h"

namespace MyWorld
{
	App::App()
	{}

	void App::Init()
	{
		WindowParams windowParams;
		Window::Init(windowParams);

		WindowSize windowSize = MyWorld::Window::getWindowSize();
		Renderer::Init({ MyWorld::Window::getWindowHWND(), { windowSize.width, windowSize.height } });
		Camera::Init({ { windowSize.width, windowSize.height } });

		// initialize chunk
		Chunk::Init();

		Window::setEventCallback([](Event& event)
			{
				switch (event.type)
				{
				case Event::TYPE::KEY_PRESS:
					handleKeyPress((KeyPressEvent&) event);
					break;
				case Event::TYPE::MOUSE_MOVE:
					handleMouseMove((MouseMoveEvent&) event);
					break;
				default:
					break;
				}
			});
	}

	void App::Terminate()
	{		
		Chunk::Terminate();
		Renderer::Terminate();
		Camera::Terminate();

		Window::Terminate();
	}

	void App::Run()
	{
		Chunk chunk({ 0.0f, 0.0f, 0.0f });

		while (!Window::shouldWindowTerminate())
		{
			Time::updateTime();

			Window::Begin();
			Renderer::Begin();
			Camera::Begin();

			// Main logic begin
			chunk.Draw();
			// Main logic end

			if (Cursor::hideCursor) Cursor::reset();

			Window::End();
			Renderer::End();
			Camera::End();
		}
	}
}
