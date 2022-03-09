#include "EventsHandler.h"

namespace MyWorld
{
	void handleKeyPress(KeyPressEvent& event)
	{
		switch (event.keyCode)
		{
		case Key::Space:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveUp = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveUp = false;
			break;
		case Key::Z:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveDown = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveDown = false;
			break;
		case Key::A:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveLeft = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveLeft = false;
			break;
		case Key::D:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveRight = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveRight = false;
			break;
		case Key::W:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveForward = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveForward = false;
			break;
		case Key::S:
			if (event.action == KeyPressEvent::KEY_DOWN)
				Camera::moveBackward = true;
			else if (event.action == KeyPressEvent::KEY_UP)
				Camera::moveBackward = false;
			break;
		case Key::H:
			if (event.action != KeyPressEvent::KEY_PRESS)
				return;
			if (Cursor::hideCursor)
				Cursor::show();
			else
				Cursor::hide();
			Cursor::hideCursor = !Cursor::hideCursor;
			break;
		case Key::T:
			if (event.action == KeyPressEvent::KEY_PRESS) Renderer::switchRenderMode();
			break;
		case Key::O:
			if (event.action == KeyPressEvent::KEY_PRESS)
			{
				Chunk::toggleFaceCulling();
				Data::Destroy();
				Data::Init();
			}
			break;
		case Key::I:
			if (event.action == KeyPressEvent::KEY_PRESS)
			{
				const bool showInfo = Renderer::toggleDebugText();
				Info::setVisible(showInfo);
			}
			break;
		case Key::Escape:
			Window::setShouldWindowClose(true);
			break;
		default:
			break;
		}
	}

	void handleMouseMove(MouseMoveEvent& event)
	{
		Cursor::updateCoord({ event.xPos, event.yPos });
		Camera::Rotate(Cursor::getDeltaCoord());
	}
}
