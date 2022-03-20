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
}
