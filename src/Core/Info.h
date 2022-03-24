#pragma once
#include "Renderer/Renderer.h"
#include "Time.h"

namespace MyWorld
{
	class Info
	{
	private:
		static bool showInfo; // SETTINGS
	public:
		static void Update();
		static void setVisible(bool visible);
		static const bool& shouldShowInfo();
	};
}
