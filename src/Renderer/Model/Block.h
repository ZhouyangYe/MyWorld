#pragma once
#include "../Renderer.h"

namespace MyWorld
{
	class Block
	{
	private:
		struct PosColorVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
		};
		static PosColorVertex cubeVertices[];
		static const uint16_t cubeTriList[];
		glm::vec3 coords;
		static bgfx::VertexBufferHandle vbh;
		static bgfx::IndexBufferHandle ibh;
	public:
		Block();
		Block(glm::vec3 coords);
		void Draw();
		static void Init();
		static void Terminate();
	};
}
