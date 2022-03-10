#pragma once
#include "Block.h"

namespace MyWorld
{
	class Grass : public Block
	{
	private:
		static Renderer::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 side;
		static const glm::vec2 top;
		static const glm::vec2 bottom;
	public:
		static void Register();
		static void Destroy();
		static const Renderer::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	public:
		Grass();
		Grass(glm::vec3 coords, glm::vec2 chunk_coords);
		~Grass();
		void Draw(const uint8_t& faces) override;
	};
}
