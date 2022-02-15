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

		// register blocks
		Block::Register();
		Grass::Register();
		Dirt::Register();

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
		// destroy blocks
		Block::Destroy();
		Grass::Destroy();
		Dirt::Destroy();

		Renderer::Terminate();
		Camera::Terminate();

		Window::Terminate();
	}

	void App::Run()
	{
		unsigned int counter = 0;

		MyWorld::Grass blocks_1[25 * 25];
		for (uint32_t yy = 0; yy < 25; ++yy)
		{
			for (uint32_t xx = 0; xx < 25; ++xx)
			{
				blocks_1[yy * 25 + xx] = MyWorld::Grass({-10.0f + float(xx) * 1.0f, -10.0f + float(yy) * 1.0f, 0.0f});
			}
		}

		MyWorld::Dirt blocks_2[25 * 25];
		for (uint32_t yy = 0; yy < 25; ++yy)
		{
			for (uint32_t xx = 0; xx < 25; ++xx)
			{
				blocks_2[yy * 25 + xx] = MyWorld::Dirt({ -10.0f + float(xx) * 1.0f, -10.0f + float(yy) * 1.0f, -1.0f });
			}
		}

		while (!Window::shouldWindowTerminate())
		{
			Window::Begin();
			Renderer::Begin();
			Camera::Begin();

			Time::updateTime();

			for (uint32_t count = 0; count < 25 * 25; count++)
			{
				blocks_1[count].Draw();
				blocks_2[count].Draw();
			}

			counter++;

			if (Cursor::hideCursor)
			{
				Cursor::reset();
			}

			Window::End();
			Renderer::End();
			Camera::End();
		}
	}
}
