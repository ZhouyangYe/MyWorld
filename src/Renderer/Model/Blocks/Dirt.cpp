#include "Dirt.h"

namespace MyWorld
{
	const glm::vec2 Dirt::face = { 3.0f, 1.0f };

	const Renderer::PosTextureArrayVertex* Dirt::getFaceVertices(Block& start, Block& end, Block::DIRECTION direction)
	{
		return Model::getFaceVertices(start, end, face, direction);
	}

	void Dirt::Register()
	{}

	void Dirt::Destroy()
	{}

	Dirt::Dirt()
	{}

	Dirt::Dirt(glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id) : Block(Block::TYPE::DIRT, coords, chunk_coords, chunk_id)
	{}

	Dirt::~Dirt()
	{
		// std::cout << "hello" << "\n";
	}
}
