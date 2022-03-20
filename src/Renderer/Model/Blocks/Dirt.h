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
	};
}
