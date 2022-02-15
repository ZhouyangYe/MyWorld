#pragma once
#include "../Tools.h"

namespace MyWorld
{
	class Texture
	{
	private:
		bgfx::UniformHandle s_texColor;
	public:
		const bgfx::TextureHandle textureHandle;
		const glm::vec2 size;
		Texture(const char* name, glm::vec2 size);
		~Texture();
		void bind();
	};
}
