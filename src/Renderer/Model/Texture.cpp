#include "Texture.h"

namespace MyWorld
{
	Texture::Texture(const char* name, glm::vec2 size) : textureHandle(Tools::loadTexture(name)), size(size), s_texColor(bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler))
	{}

	Texture::~Texture()
	{
		bgfx::destroy(textureHandle);
		bgfx::destroy(s_texColor);
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
}
