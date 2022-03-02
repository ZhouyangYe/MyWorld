#pragma once
#include "Renderer/Renderer.h"
#include "Time.h"

namespace MyWorld
{
	class Info
	{
	private:
		static bool showInfo;
	public:
		static void Update();
		static void setVisible(bool visible);
	};
}
