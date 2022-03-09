#include "Grass.h"

namespace MyWorld
{
	Renderer::PosTextureVertex* Grass::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Grass::vbh = BGFX_INVALID_HANDLE;
	const glm::vec2 Grass::side = { 2.0f, 1.0f };
	const glm::vec2 Grass::top = { 1.0f, 1.0f };
	const glm::vec2 Grass::bottom = { 3.0f, 1.0f };

	const Renderer::PosTextureArrayVertex* Grass::getFaceVertices(Block* start, Block* end, Block::DIRECTION direction)
	{
		switch (direction)
		{
		case MyWorld::Block::NORTH:
		case MyWorld::Block::SOUTH:
		case MyWorld::Block::WEST:
		case MyWorld::Block::EAST:
			return Block::getFaceVertices(start, end, side, direction);
		case MyWorld::Block::TOP:
			return Block::getFaceVertices(start, end, top, direction);
		case MyWorld::Block::BOTTOM:
			return Block::getFaceVertices(start, end, bottom, direction);
		default:
			return nullptr;
		}
	}

	void Grass::Register()
	{
		if (!Texture::isArrayBufferSupported())
		{
			cubeVertices = Block::getVerticesType1(side, top, bottom);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Renderer::PosTextureVertex)), Renderer::PosTextureVertex::layout);
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
		Block::Draw(vbh, Block::getIbh(faces), Renderer::texture_program);
	}
}
