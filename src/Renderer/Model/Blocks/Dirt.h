#pragma once
#include "../Model.h"

namespace MyWorld
{
	class Dirt : public Block
	{
	private:
		static const glm::vec2 face;
	public:
		static void Register();
		static void Destroy();
		static const Renderer::PosTextureArrayVertex* getFaceVertices(Block& start, Block& end, Block::DIRECTION direction);
	public:
		Dirt();
		Dirt(glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id = 0);
		~Dirt();
	};
}
