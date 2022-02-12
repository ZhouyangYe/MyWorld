#pragma once
#include "Util.h";
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"
#include "Cursor.h"

namespace MyWorld
{
	void handleKeyPress(KeyPressEvent& event);
	void handleMouseMove(MouseMoveEvent& event);
}
