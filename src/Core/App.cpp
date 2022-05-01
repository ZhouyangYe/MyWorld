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
		Model::Register();
		Grass::Register();
		Dirt::Register();
		Water::Register();
		Wireframe::Register();
		HitBox::Register();

		// initialize chunk
		Chunk::Init();

		// initialize world
		World::Init();

		Camera::Init({ { windowSize.width, windowSize.height }, (float)World::getRenderDistance() * 16, Chunk::getSpawnLocation() });
		Player::Register();

		Window::setEventCallback([](Event& event)
			{
				switch (event.type)
				{
				case Event::TYPE::KEY_PRESS:
					EventHandler::handleKeyPress((KeyPressEvent&) event);
					break;
				case Event::TYPE::MOUSE_MOVE:
					EventHandler::handleMouseMove((MouseMoveEvent&) event);
					break;
				case Event::TYPE::MOUSE_WHEEL:
					EventHandler::handleMouseWheel((MouseWheelEvent&) event);
					break;
				default:
					break;
				}
			});
		
		// update prevTime right before rendering starts
		Time::Init();
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

		Camera::End();
		Renderer::End();
		Window::End();

		// TODO: lock framerate
	}

	void App::Terminate()
	{
		// destroy blocks
		Model::Destroy();
		Grass::Destroy();
		Dirt::Destroy();
		Water::Destroy();
		Wireframe::Destroy();
		HitBox::Destroy();

		// destroy chunk
		Chunk::Destroy();

		// destroy world
		World::Destroy();
		Renderer::Terminate();
		Camera::Terminate();
		Player::Destroy();

		Window::Terminate();
	}
}
