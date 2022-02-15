#pragma once
#include "../../Renderer.h"
#include "../Texture.h"

#define xUnit 1/16
#define yUnit 1/16

namespace MyWorld
{
	class Block
	{
	protected:
		struct PosTextureVertex
		{
			float x;
			float y;
			float z;
			float u;
			float v;
		};
		static PosTextureVertex* getVerticesType1(glm::vec2 &side, glm::vec2 &top, glm::vec2 &bottom);
		static bgfx::IndexBufferHandle ibh_t1;
	private:
		static const uint16_t cubeTriListType1[];
		glm::vec3 coords;
		static Texture* texture;
	public:
		Block();
		Block(glm::vec3 coords);
		void Draw(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh);
		static void Register();
		static void Destroy();
		virtual void Draw() = 0;
	};
}
