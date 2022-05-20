#pragma once
#include "Util.h";
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"
#include "Cursor.h"
#include "Renderer/Renderer.h"
#include "World/Chunk.h"
#include "Info.h"
#include "World/World.h"

namespace MyWorld
{
	namespace EventHandler
	{
		void handleKeyPress(KeyPressEvent& event);
		void handleMouseMove(MouseMoveEvent& event);
		void handleMouseWheel(MouseWheelEvent& event);
	}
}
