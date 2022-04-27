#include "EventsHandler.h"

namespace MyWorld
{
	namespace EventHandler
	{
		void handleKeyPress(KeyPressEvent& event)
		{
			switch (event.keyCode)
			{
			case Key::Space:
				if (!Camera::freeCamera) break;
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
				{
					Camera::moveUp = true;
					World::getPlayer().clearVelocity();
					if (World::gravityEnabled) World::gravityEnabled = false;
					if (Camera::cameraMoveDisabled) Camera::cameraMoveDisabled = false;
				}
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveUp = false;
				break;
			case Key::Z:
				if (!Camera::freeCamera) break;
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
				{
					Camera::moveDown = true;
					World::getPlayer().clearVelocity();
					if (World::gravityEnabled) World::gravityEnabled = false;
					if (Camera::cameraMoveDisabled) Camera::cameraMoveDisabled = false;
				}
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveDown = false;
				break;
			case Key::A:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					Camera::moveLeft = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveLeft = false;
				break;
			case Key::D:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					Camera::moveRight = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveRight = false;
				break;
			case Key::W:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					Camera::moveForward = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveForward = false;
				break;
			case Key::S:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					Camera::moveBackward = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					Camera::moveBackward = false;
				break;
			case Key::H:
				if (event.action != KeyPressEvent::ACTION::KEY_DOWN)
					break;
				if (Cursor::hideCursor)
					Cursor::show();
				else
					Cursor::hide();
				Cursor::hideCursor = !Cursor::hideCursor;
				break;
			case Key::G:
				if (event.action == KeyPressEvent::ACTION::KEY_UP)
					World::gravityEnabled = !World::gravityEnabled;
				break;
			case Key::T: // show
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN) Renderer::switchRenderMode();
				break;
			case Key::F1:
				if (event.action == KeyPressEvent::ACTION::KEY_UP)
				{
					const bool showInfo = Renderer::toggleDebugText();
					Info::setVisible(showInfo);
				}
				break;
			case Key::Escape:
				World::gameover = true;
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

		void handleMouseWheel(MouseWheelEvent& event)
		{
			if (event.yOffset < 0)
				Camera::zoomOut();
			else
				Camera::zoomIn();
		}
	}
}
