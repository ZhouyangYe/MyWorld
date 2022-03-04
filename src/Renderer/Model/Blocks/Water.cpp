#include "Water.h"

namespace MyWorld
{
	Block::PosColorTextureArrayVertex* Water::cubeVertices_ta = nullptr;
	Block::PosColorTextureVertex* Water::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Water::vbh = BGFX_INVALID_HANDLE;

	void Water::Register()
	{
		const glm::vec2 face{ 1.0f, 16.0f };
		const uint32_t color = 0xaa000000;

		if (Texture::isArrayBufferSupported())
		{
			cubeVertices_ta = Block::getVerticesType4(face, face, face, color);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices_ta, 16 * sizeof(Block::PosColorTextureArrayVertex)), Renderer::getColorTextureArrayLayout());
		}
		else
		{
			cubeVertices = Block::getVerticesType2(face, face, face, color);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosColorTextureVertex)), Renderer::getColorTextureLayout());
		}
	}

	void Water::Destroy()
	{
		bgfx::destroy(vbh);
		delete[] cubeVertices;
		cubeVertices = nullptr;
		delete[] cubeVertices_ta;
		cubeVertices_ta = nullptr;
	}

	Water::Water()
	{}

	Water::Water(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::WATER, coords, chunk_coords), program(Texture::isArrayBufferSupported() ? Renderer::texture_array_color_program : Renderer::texture_color_program)
	{
	}

	Water::~Water()
	{}

	void Water::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program, Block::default_state & (~BGFX_STATE_CULL_CW));
	}
}
