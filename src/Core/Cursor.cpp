#include "Cursor.h"

namespace MyWorld
{
	bool Cursor::hideCursor = false;
	float Cursor::sensitivity = 0.5f;
	glm::vec2 Cursor::prevCoord;
	glm::vec2 Cursor::delta = { 0.0f, 0.0f };

	glm::vec2 Cursor::getDeltaCoord()
	{
		return delta * sensitivity;
	}

	void Cursor::updateCoord(glm::vec2 coord)
	{
		WindowSize center = Window::getCenter();
		if (prevCoord.length == 0)
			prevCoord = coord;

		delta = coord - prevCoord;
		prevCoord = hideCursor ? glm::vec2{ center.width, center.height } : coord;
	}

	void Cursor::hide()
	{
		WindowSize center = Window::getCenter();
		prevCoord = glm::vec2{ center.width, center.height };
		glfwSetInputMode(Window::getGlfwWindowObj(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Cursor::reset()
	{
		WindowSize center = Window::getCenter();
		glfwSetCursorPos(Window::getGlfwWindowObj(), center.width, center.height);
	}

	void Cursor::show()
	{
		glfwSetInputMode(Window::getGlfwWindowObj(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
