#pragma once
#include "Block.h"

namespace MyWorld
{
	class Wireframe : public Block
	{
	private:
		static Renderer::PosColorVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const uint8_t faces;
		static const uint64_t state;
		static const uint64_t state_placeholder;
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
