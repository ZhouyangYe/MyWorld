#pragma once
#include "../../Renderer.h"
#include "../../Camera.h"
#include "../Texture.h"

namespace MyWorld
{
	class Player
	{
	public:
		static void Register();
		static void Destroy();
		static void Draw();
	};
}
