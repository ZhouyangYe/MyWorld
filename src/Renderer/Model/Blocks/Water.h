#pragma once
#include "../Model.h"

namespace MyWorld
{
	class Water : public Block
	{
	private:
		static const glm::vec2 face;
	public:
		static const uint64_t state;
		static const uint64_t placeholder_state;
		static void Register();
		static void Destroy();
		static const Renderer::PosTextureArrayVertex* getFaceVertices(Block& start, Block& end, Block::DIRECTION direction);
	public:
		Water();
		Water(glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id = 0);
		~Water();
	};
}
