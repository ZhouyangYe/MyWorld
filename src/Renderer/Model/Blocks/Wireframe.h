#pragma once
#include "Block.h"

namespace MyWorld
{
	class Wireframe : public Block
	{
	private:
		static Renderer::PosTextureArrayVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
		static const uint8_t faces;
	public:
		static void Register();
		static void Destroy();
	public:
		Wireframe();
		Wireframe(glm::vec3 coords, glm::vec2 chunk_coords);
		~Wireframe();
		void Draw(const uint8_t& faces = faces) override;
		void setCoords(glm::vec3& coords);
	};
}
