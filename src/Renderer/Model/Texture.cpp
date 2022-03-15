#include "Texture.h"

namespace MyWorld
{
	bool Texture::arrayBufferSupported = false;
	bgfx::UniformHandle Texture::s_texColor = BGFX_INVALID_HANDLE;

	Texture::Texture(const char* name) : textureHandle(BGFX_INVALID_HANDLE)
	{
		textureHandle = Tools::loadTexture(name, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &info);
	}

	Texture::Texture(const char* name, Tools::TextureArrayParam taInfo) : textureHandle(BGFX_INVALID_HANDLE)
	{
		textureHandle = Tools::loadTexture(name, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &info, NULL, &taInfo);
	}

	Texture::~Texture()
	{
		if (bgfx::isValid(textureHandle)) bgfx::destroy(textureHandle);
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

	void Texture::Init()
	{
		arrayBufferSupported = 0 != (BGFX_CAPS_TEXTURE_2D_ARRAY & Tools::caps->supported);
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	}

	void Texture::Destroy()
	{
		bgfx::destroy(s_texColor);
	}

	const bool& Texture::isArrayBufferSupported()
	{
		return arrayBufferSupported;
	}
}
