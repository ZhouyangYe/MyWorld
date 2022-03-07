#include "Texture.h"

namespace MyWorld
{
	bool Texture::arrayBufferSupported = false;
	bgfx::TextureHandle Texture::fBufferTexture_water = BGFX_INVALID_HANDLE;
	bgfx::FrameBufferHandle Texture::fbh_water = BGFX_INVALID_HANDLE;
	bgfx::UniformHandle Texture::s_texColor = BGFX_INVALID_HANDLE;

	Texture::Texture(const char* name, Tools::TextureArrayParam taInfo) : textureHandle(BGFX_INVALID_HANDLE)
	{
		if (arrayBufferSupported)
		{
			textureHandle = Tools::loadTexture(name, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &info, NULL, &taInfo);
		}
		else
		{
			textureHandle = Tools::loadTexture(name, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &info);
		}
	}

	Texture::~Texture()
	{
		bgfx::destroy(textureHandle);
	}

	const bgfx::TextureInfo& Texture::getInfo()
	{
		return info;
	}

	void Texture::bind()
	{
		bgfx::setTexture(0, s_texColor, textureHandle, 0
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
		);
	}

	const bgfx::TextureHandle& Texture::getTextureHandle()
	{
		return textureHandle;
	}

	void Texture::Init(TextureParam param)
	{
		arrayBufferSupported = 0 != (BGFX_CAPS_TEXTURE_2D_ARRAY & bgfx::getCaps()->supported);
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

		if (arrayBufferSupported)
		{
		  // create frame buffer for water and bind to water view
		  fBufferTexture_water = bgfx::createTexture2D(uint16_t(param.window_width), uint16_t(param.window_height), false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
		  fbh_water = bgfx::createFrameBuffer(1, &fBufferTexture_water, true);
		}
	}

	void Texture::Destroy()
	{
		bgfx::destroy(s_texColor);
		if (bgfx::isValid(fBufferTexture_water)) bgfx::destroy(fBufferTexture_water);
		if (bgfx::isValid(fbh_water)) bgfx::destroy(fbh_water);
	}

	const bgfx::UniformHandle& Texture::getTexColorSampler()
	{
		return s_texColor;
	}

	const bgfx::TextureHandle& Texture::getWaterTextureHandle()
	{
		return fBufferTexture_water;
	}

	const bgfx::FrameBufferHandle& Texture::getWaterFbh()
	{
		return fbh_water;
	}

	const bool& Texture::isArrayBufferSupported()
	{
		return arrayBufferSupported;
	}
}
