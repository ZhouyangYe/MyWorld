#include "Cursor.h"

namespace MyWorld
{
	bool Cursor::hideCursor = false;
	float Cursor::sensitivity = 0.2f;
	glm::vec2 Cursor::prevCoord;
	glm::vec2 Cursor::delta = { 0.0f, 0.0f };

	glm::vec2 Cursor::getDeltaCoord()
	{
		return delta * sensitivity;
	}

	void Cursor::updateCoord(glm::vec2 coord)
	{
		glm::vec2 center = Window::getCenter();
		if (prevCoord.length == 0)
			prevCoord = coord;

		delta = coord - prevCoord;
		prevCoord = hideCursor ? center : coord;
	}

	void Cursor::hide()
	{
		glm::vec2 center = Window::getCenter();
		prevCoord = glm::vec2{ center.x, center.y };
		Window::hideCursor();
	}

	void Cursor::reset()
	{
		glm::vec2 center = Window::getCenter();
		Window::setCursorPos(center.x, center.y);
	}

	void Cursor::show()
	{
		Window::showCursor();
	}
}
