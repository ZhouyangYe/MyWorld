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

		// initialize app data
		Data::Init();

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
		Data::Destroy();
		Renderer::Terminate();
		Camera::Terminate();

		Window::Terminate();
	}

	void App::Run()
	{
		Time::updateTime();

		Window::Begin();
		Renderer::Begin();
		Camera::Begin();

		for (int i = 0; i < Data::CHUNK_WIDTH_NUM * Data::CHUNK_WIDTH_NUM; i++)
		{
			Data::chunks[i]->Draw();
		}
		// Data::chunks[0]->Draw();

		Info::Update();
		if (Cursor::hideCursor) Cursor::reset();

		Window::End();
		Renderer::End();
		Camera::End();
	}
}
