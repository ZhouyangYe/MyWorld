#pragma once
#include "../Tools.h"

namespace MyWorld
{
	class Texture
	{
	private:
		bgfx::UniformHandle s_texColor;
		static bool arrayBufferSupported;
	public:
		static const bool& isArrayBufferSupported();
		const bgfx::TextureHandle textureHandle;
		const glm::vec2 size;
		Texture(const char* name, glm::vec2 size);
		~Texture();
		void bind();
		static void Init();
		static void Destroy();
	};
}
