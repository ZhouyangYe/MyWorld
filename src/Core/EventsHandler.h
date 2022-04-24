#pragma once
#include "Util.h";
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"
#include "Cursor.h"
#include "Renderer/Renderer.h"
#include "Chunk.h"
#include "Info.h"
#include "World.h"

namespace MyWorld
{
	void handleKeyPress(KeyPressEvent& event);
	void handleMouseMove(MouseMoveEvent& event);
	void handleMouseWheel(MouseWheelEvent& event);
}
