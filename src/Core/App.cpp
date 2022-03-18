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

		// register blocks
		Block::Register();
		Grass::Register();
		Dirt::Register();
		Water::Register();
		Wireframe::Register();

		// initialize chunk
		Chunk::Init();

		// initialize world
		World::Generate();

		Camera::Init({ { windowSize.width, windowSize.height }, (float)World::getRenderDistance() * 16, Chunk::getSpawnLocation() });

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
		Water::Destroy();
		Wireframe::Destroy();

		// destroy chunk
		Chunk::Destroy();

		// destroy world
		World::Destroy();
		Renderer::Terminate();
		Camera::Terminate();

		Window::Terminate();
	}

	void App::Update()
	{
		Time::updateTime();
		if (Info::shouldShowInfo()) Time::updateFps();

		Window::Begin();
		Renderer::Begin();
		Camera::Begin();

		// main game logic
		World::Update();

		Info::Update();
		if (Cursor::hideCursor) Cursor::reset();

		Window::End();
		Renderer::End();
		Camera::End();
	}
}
