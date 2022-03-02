#pragma once
#include "Block.h"
#include "../../Camera.h"

namespace MyWorld
{
	class Water : public Block
	{
	private:
		bgfx::ProgramHandle program;
		static Block::PosColorTextureArrayVertex* cubeVertices_ta;
		static Block::PosColorTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Water();
		Water(glm::vec3 coords);
		~Water();
		void Draw(const uint8_t& faces) override;
	};
}
