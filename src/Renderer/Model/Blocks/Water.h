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
	};
}
