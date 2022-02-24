#pragma once
#include "Block.h"

namespace MyWorld
{
	class Grass : public Block
	{
	private:
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Grass();
		Grass(glm::vec3 coords);
		~Grass();
		void Draw(const uint8_t& faces) override;
	};
}
