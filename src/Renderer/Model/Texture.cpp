#include "Texture.h"

namespace MyWorld
{
	bool Texture::arrayBufferSupported = false;

	Texture::Texture(const char* name, Tools::TextureArrayParam taInfo) : textureHandle(BGFX_INVALID_HANDLE), s_texColor(bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler))
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
		bgfx::destroy(s_texColor);
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
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
		);
	}

	const bgfx::TextureHandle& Texture::getTextureHandle()
	{
		return textureHandle;
	}

	void Texture::Init()
	{
		arrayBufferSupported = 0 != (BGFX_CAPS_TEXTURE_2D_ARRAY & bgfx::getCaps()->supported);
	}

	void Texture::Destroy()
	{}

	const bool& Texture::isArrayBufferSupported()
	{
		return arrayBufferSupported;
	}
}
