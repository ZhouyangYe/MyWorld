#pragma once
#include "../Renderer.h"
#include "Texture.h"

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
		static PosTextureVertex cubeVertices[];
	private:
		static const uint16_t cubeTriList[];
		glm::vec3 coords;
		static bgfx::VertexBufferHandle vbh;
		static bgfx::IndexBufferHandle ibh;
		static bgfx::UniformHandle s_texColor;
		static bgfx::TextureHandle m_textureColor;
	public:
		Block();
		Block(glm::vec3 coords);
		void Draw();
		static void Init();
		static void Terminate();
	};
}
