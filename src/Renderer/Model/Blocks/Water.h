#pragma once
#include "Block.h"
#include "../../Camera.h"

namespace MyWorld
{
	class Water : public Block
	{
	private:
		static bgfx::ProgramHandle program;
		static Renderer::PosColorTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
		static const uint32_t color;
	public:
		static const uint64_t state;
		static void Register();
		static void Destroy();
		Water();
		Water(glm::vec3 coords, glm::vec2 chunk_coords);
		~Water();
		void Draw(const uint8_t& faces) override;
		static const Renderer::PosColorTextureArrayVertex* getFaceVertices(Block* start, Block* end, Block::DIRECTION direction);
	};
}
