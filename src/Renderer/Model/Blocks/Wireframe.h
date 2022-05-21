#pragma once
#include "../Model.h"

namespace MyWorld
{
	class Wireframe : public Block
	{
	private:
		static Renderer::PosColorVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const uint64_t state;
		static const uint8_t faces;
	public:
		static void Register();
		static void Destroy();
	public:
		Wireframe();
		~Wireframe();
		void Draw();
		void setCoords(const glm::vec3& coords);
	};
}
