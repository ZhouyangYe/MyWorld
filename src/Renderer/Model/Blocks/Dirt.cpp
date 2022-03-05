#include "Dirt.h"

namespace MyWorld
{
	Block::PosTextureVertex* Dirt::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Dirt::vbh = BGFX_INVALID_HANDLE;
	bgfx::ProgramHandle Dirt::program = BGFX_INVALID_HANDLE;
	const glm::vec2 Dirt::face = { 3.0f, 1.0f };

	const Block::PosTextureArrayVertex* Dirt::getFaceVertices(Block* start, Block* end, Block::DIRECTION direction)
	{
		return Block::getFaceVerticesType1(start, end, face, direction);
	}

	void Dirt::Register()
	{
		if (!Texture::isArrayBufferSupported())
		{
			cubeVertices = Block::getVerticesType1(face, face, face);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
			program = Renderer::texture_program;
		}
	}

	void Dirt::Destroy()
	{
		if (bgfx::isValid(vbh)) bgfx::destroy(vbh);
		if (cubeVertices != nullptr)
		{
			delete[] cubeVertices;
			cubeVertices = nullptr;
		}
	}

	Dirt::Dirt()
	{}

	Dirt::Dirt(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::DIRT, coords, chunk_coords)
	{
	}

	Dirt::~Dirt()
	{}

	void Dirt::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program);
	}
}
