#include "Texture.h"

namespace MyWorld
{
	bool Texture::arrayBufferSupported = false;
	bgfx::TextureHandle Texture::oitWaterTextHandle = BGFX_INVALID_HANDLE;
	bgfx::FrameBufferHandle Texture::oitWaterFbh = BGFX_INVALID_HANDLE;
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

	void Texture::bindOitWaterTexture()
	{
		bgfx::setTexture(0, s_texColor, oitWaterTextHandle);
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
		  // create frame buffer texture for water and bind to water view
			oitWaterTextHandle = bgfx::createTexture2D(uint16_t(param.window_width), uint16_t(param.window_height), false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
			oitWaterFbh = bgfx::createFrameBuffer(1, &oitWaterTextHandle, true);
			bgfx::setViewFrameBuffer(Tools::OIT_WATER_VIEW_ID, oitWaterFbh);
		}
	}

	void Texture::Destroy()
	{
		bgfx::destroy(s_texColor);
		if (bgfx::isValid(oitWaterTextHandle)) bgfx::destroy(oitWaterTextHandle);
		if (bgfx::isValid(oitWaterFbh)) bgfx::destroy(oitWaterFbh);
	}

	const bool& Texture::isArrayBufferSupported()
	{
		return arrayBufferSupported;
	}
}
