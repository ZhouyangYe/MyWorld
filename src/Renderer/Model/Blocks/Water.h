#pragma once
#include "Block.h"
#include "../../Camera.h"

namespace MyWorld
{
	class Water : public Block
	{
	private:
		static Renderer::PosColorTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
		static const uint32_t color;
	public:
		static const uint64_t state;
		static const uint64_t placeholder_state;
		static void Register();
		static void Destroy();
		static const Renderer::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	public:
		const int chunk_id;
		Water();
		Water(glm::vec3 coords, glm::vec2 chunk_coords, const int chunk_id);
		~Water();
		void Draw(const uint8_t& faces) override;
	};
}
