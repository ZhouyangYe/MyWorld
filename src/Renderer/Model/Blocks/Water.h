#pragma once
#include "Block.h"

namespace MyWorld
{
	class Water : public Block
	{
	private:
		static Block::PosColorTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Water();
		Water(glm::vec3 coords);
		~Water();
		void Draw(uint8_t& faces) override;
	};
}
