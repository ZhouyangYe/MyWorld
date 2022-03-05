#pragma once
#include "Block.h"

namespace MyWorld
{
	class Grass : public Block
	{
	private:
		static bgfx::ProgramHandle program;
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 side;
		static const glm::vec2 top;
		static const glm::vec2 bottom;
	public:
		static void Register();
		static void Destroy();
		Grass();
		Grass(glm::vec3 coords, glm::vec2 chunk_coords);
		~Grass();
		void Draw(const uint8_t& faces) override;
		static const Block::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	};
}
