#pragma once
#include "Block.h"

namespace MyWorld
{
	class Grass : public Block
	{
	private:
		bgfx::ProgramHandle program;
		static Block::PosTextureArrayVertex* cubeVertices_ta;
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Grass();
		Grass(glm::vec3 coords, glm::vec2 chunk_coords);
		~Grass();
		void Draw(const uint8_t& faces) override;
	};
}
