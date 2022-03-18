#include "Grass.h"

namespace MyWorld
{
	const glm::vec2 Grass::side = { 2.0f, 1.0f };
	const glm::vec2 Grass::top = { 1.0f, 1.0f };
	const glm::vec2 Grass::bottom = { 3.0f, 1.0f };

	const Renderer::PosTextureArrayVertex* Grass::getFaceVertices(Block* start, Block* end, Block::DIRECTION direction)
	{
		switch (direction)
		{
		case Block::DIRECTION::NORTH:
		case Block::DIRECTION::SOUTH:
		case Block::DIRECTION::WEST:
		case Block::DIRECTION::EAST:
			return Block::getFaceVertices(start, end, side, direction);
		case Block::DIRECTION::TOP:
			return Block::getFaceVertices(start, end, top, direction);
		case Block::DIRECTION::BOTTOM:
			return Block::getFaceVertices(start, end, bottom, direction);
		default:
			return nullptr;
		}
	}

	void Grass::Register()
	{}

	void Grass::Destroy()
	{}

	Grass::Grass()
	{}

	Grass::Grass(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::TYPE::GRASS, coords, chunk_coords)
	{}

	Grass::~Grass()
	{}

	void Grass::Draw(const uint8_t& faces)
	{}
}
