#pragma once
#include "Block.h"

namespace MyWorld
{
	class Dirt : public Block
	{
	private:
		static Block::PosTextureVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
	public:
		static void Register();
		static void Destroy();
		Dirt();
		Dirt(glm::vec3 coords);
		~Dirt();
		void Draw() override;
	};
}