#pragma once
#include "../Renderer.h"

namespace MyWorld
{
	class Texture
	{
	public:
		const bgfx::TextureHandle texture;
		const glm::vec2 texCoords[];
		Texture(const char* name, glm::vec2 texCoords[]);
		~Texture();
	};
}
