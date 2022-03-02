#pragma once
#include "../Tools.h"

namespace MyWorld
{
	class Texture
	{
	private:
		bgfx::UniformHandle s_texColor;
		bgfx::TextureHandle textureHandle;
		static bool arrayBufferSupported;
		bgfx::TextureInfo info;
	public:
		static const bool& isArrayBufferSupported();
		Texture(const char* name, Tools::TextureArrayParam taInfo);
		~Texture();
		void bind();
		const bgfx::TextureHandle& getTextureHandle();
		static void Init();
		static void Destroy();
	};
}
