#pragma once
#include "../Model.h"

namespace MyWorld
{
	class Wireframe : public Block
	{
	private:
		static Renderer::PosTextureArrayVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const glm::vec2 face;
		static const uint8_t faces;
		static const uint64_t state;
	public:
		static void Register();
		static void Destroy();
	public:
		Wireframe();
		~Wireframe();
		void Draw(const uint8_t& faces = faces);
		void setCoords(const glm::vec3& coords);
	};
}
