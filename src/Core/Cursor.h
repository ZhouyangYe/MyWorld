#pragma once
#include "Window/Window.h"

namespace MyWorld
{
	class Cursor
	{
	public:
		static bool hideCursor; // SETTINGS
		static glm::vec2 getDeltaCoord();
		static void updateCoord(glm::vec2 coord);
		static void hide();
		static void reset();
		static void show();
	private:
		static float sensitivity; // SETTINGS
		static glm::vec2 prevCoord;
		static glm::vec2 delta;
	};
}
