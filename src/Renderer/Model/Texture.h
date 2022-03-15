#pragma once
#include "../Tools.h"

namespace MyWorld
{
	class Texture
	{
	private:
		static bgfx::UniformHandle s_texColor;
		bgfx::TextureHandle textureHandle;
		bgfx::TextureInfo info;
		static bool arrayBufferSupported;
	public:
		struct TextureParam
		{
			int window_width;
			int window_height;
		};
		static const bool& isArrayBufferSupported();
		Texture(const char* path);
		Texture(const char* path, Tools::TextureArrayParam taInfo);
		~Texture();
		void bind();
		const bgfx::TextureHandle& getTextureHandle();
		const bgfx::TextureInfo& getInfo();
		static void Init();
		static void Destroy();
	};
}
