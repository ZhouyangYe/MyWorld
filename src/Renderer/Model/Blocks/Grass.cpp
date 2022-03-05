#include "Grass.h"

namespace MyWorld
{
	Block::PosTextureVertex* Grass::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Grass::vbh = BGFX_INVALID_HANDLE;
	bgfx::ProgramHandle Grass::program = BGFX_INVALID_HANDLE;
	const glm::vec2 Grass::side = { 2.0f, 1.0f };
	const glm::vec2 Grass::top = { 1.0f, 1.0f };
	const glm::vec2 Grass::bottom = { 3.0f, 1.0f };

	const Block::PosTextureArrayVertex* Grass::getFaceVertices(Block* start, Block* end, Block::DIRECTION direction)
	{
		switch (direction)
		{
		case MyWorld::Block::NORTH:
		case MyWorld::Block::SOUTH:
		case MyWorld::Block::WEST:
		case MyWorld::Block::EAST:
			return Block::getFaceVerticesType1(start, end, side, direction);
		case MyWorld::Block::TOP:
			return Block::getFaceVerticesType1(start, end, top, direction);
		case MyWorld::Block::BOTTOM:
			return Block::getFaceVerticesType1(start, end, bottom, direction);
		default:
			return nullptr;
		}
	}

	void Grass::Register()
	{
		if (!Texture::isArrayBufferSupported())
		{
			cubeVertices = Block::getVerticesType1(side, top, bottom);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
			program = Renderer::texture_program;
		}
	}

	void Grass::Destroy()
	{
		if (bgfx::isValid(vbh)) bgfx::destroy(vbh);
		if (cubeVertices != nullptr)
		{
			delete[] cubeVertices;
			cubeVertices = nullptr;
		}
	}

	Grass::Grass()
	{}

	Grass::Grass(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::GRASS, coords, chunk_coords)
	{
	}

	Grass::~Grass()
	{}

	void Grass::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program);
	}
}
