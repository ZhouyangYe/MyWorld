#include "Water.h"

namespace MyWorld
{
	Renderer::PosColorTextureVertex* Water::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Water::vbh = BGFX_INVALID_HANDLE;
	bgfx::ProgramHandle Water::program = BGFX_INVALID_HANDLE;
	const glm::vec2 Water::face = { 1.0f, 16.0f };
	const uint32_t Water::color = 0xff000000;
	const uint64_t Water::state = Block::default_state & (~BGFX_STATE_CULL_CW);

	const Renderer::PosColorTextureArrayVertex* Water::getFaceVertices(Block* start, Block* end, Block::DIRECTION direction)
	{
		return Block::getFaceVerticesType2(start, end, face, color, direction);
	}

	void Water::Register()
	{
		if (!Texture::isArrayBufferSupported())
		{
			cubeVertices = Block::getVerticesType2(face, face, face, color);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Renderer::PosColorTextureVertex)), Renderer::PosColorTextureVertex::layout);
			program = Renderer::texture_color_program;
		}
	}

	void Water::Destroy()
	{
		if (bgfx::isValid(vbh)) bgfx::destroy(vbh);
		if (cubeVertices != nullptr)
		{
			delete[] cubeVertices;
			cubeVertices = nullptr;
		}
	}

	Water::Water()
	{}

	Water::Water(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::WATER, coords, chunk_coords)
	{
	}

	Water::~Water()
	{}

	void Water::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program, state);
	}
}
