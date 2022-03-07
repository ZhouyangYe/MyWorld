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
		static bgfx::TextureHandle fBufferTexture_water;
		static bgfx::FrameBufferHandle fbh_water;
	public:
		struct TextureParam
		{
			int window_width;
			int window_height;
		};
		static const bool& isArrayBufferSupported();
		Texture(const char* name);
		Texture(const char* name, Tools::TextureArrayParam taInfo);
		~Texture();
		void bind();
		const bgfx::TextureHandle& getTextureHandle();
		const bgfx::TextureInfo& getInfo();
		static void Init(TextureParam param);
		static void Destroy();
		static const bgfx::UniformHandle& getTexColorSampler();
		static const bgfx::TextureHandle& getWaterTextureHandle();
		static const bgfx::FrameBufferHandle& getWaterFbh();
	};
}
