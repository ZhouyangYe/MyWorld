#pragma once
#include "Block.h"

namespace MyWorld
{
	class Dirt : public Block
	{
	private:
		static bgfx::ProgramHandle program;
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
	public:
		static void Register();
		static void Destroy();
		Dirt();
		Dirt(glm::vec3 coords, glm::vec2 chunk_coords);
		~Dirt();
		void Draw(const uint8_t& faces) override;
		static const Block::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	};
}
