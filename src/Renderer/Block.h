#pragma once
#include "Renderer.h"

namespace MyWorld
{
	class Block
	{
	private:
		struct Transform
		{
			float x, y, z;
		};
		struct PosColorVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
		};
		static PosColorVertex cubeVertices[];
		static const uint16_t cubeTriList[];
		Transform coords;
		Transform rotate;
		static bgfx::VertexBufferHandle vbh;
		static bgfx::IndexBufferHandle ibh;
	public:
		struct BlockParams
		{
			Transform coords;
			Transform rotate;
		};
		Block();
		Block(BlockParams params);
		void Draw();
		static void Init();
		static void Terminate();
	};
}
