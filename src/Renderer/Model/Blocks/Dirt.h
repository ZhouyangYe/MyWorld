#pragma once
#include "Block.h"

namespace MyWorld
{
	class Dirt : public Block
	{
	private:
		bgfx::ProgramHandle program;
		static Block::PosTextureArrayVertex* cubeVertices_ta;
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Dirt();
		Dirt(glm::vec3 coords);
		~Dirt();
		void Draw(const uint8_t& faces) override;
	};
}
