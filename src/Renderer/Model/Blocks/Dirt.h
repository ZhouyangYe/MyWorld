#pragma once
#include "Block.h"

namespace MyWorld
{
	class Dirt : public Block
	{
	private:
		static Renderer::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
	public:
		static void Register();
		static void Destroy();
		Dirt();
		Dirt(glm::vec3 coords, glm::vec2 chunk_coords);
		~Dirt();
		void Draw(const uint8_t& faces) override;
		static const Renderer::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	};
}
