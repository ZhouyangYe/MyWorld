#include "Texture.h"

namespace MyWorld
{
	const bgfx::TextureHandle Texture::texture;

	Texture::Texture(const char* name, glm::vec2 texCoords[]) : texture(Renderer::loadTexture(name), texCoords(texCoords))
	{}

	Texture::~Texture()
	{
		bgfx::destroy(texture);
	}
}
