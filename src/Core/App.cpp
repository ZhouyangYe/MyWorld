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
		Block::Init();
	}

	void App::Terminate()
	{
		Renderer::Terminate();
		Camera::Terminate();
		Block::Terminate();

		Window::Terminate();
	}

	void App::Run()
	{
		unsigned int counter = 0;

		MyWorld::Block blocks[11 * 11];
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				blocks[yy * 11 + xx] = MyWorld::Block({ {-10.0f + float(xx) * 2.0f, -10.0f + float(yy) * 2.0f, xx * 1.2f + yy * 1.2f}, {counter * 0.01f + xx * 0.21f, counter * 0.0081f + yy * 0.37f, 0.0f} });
			}
		}

		while (!Window::shouldWindowTerminate())
		{
			Window::Begin();

			Renderer::Begin();

			Camera::Begin();

			for (uint32_t count = 0; count < 11 * 11; count++)
			{
				blocks[count].Draw();
			}

			counter++;
			Window::End();

			Renderer::End();

			Camera::End();
		}
	}
}
