#include "Wireframe.h"

namespace MyWorld
{
	Renderer::PosColorVertex* Wireframe::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Wireframe::vbh = BGFX_INVALID_HANDLE;
	const uint64_t Wireframe::state = Block::default_state & (~BGFX_STATE_CULL_CW) & (~BGFX_STATE_DEPTH_TEST_LESS) | BGFX_STATE_PT_LINESTRIP;
	const uint8_t Wireframe::faces = 0;

	void Wireframe::Register()
	{
		cubeVertices = Block::getVerticesType0(0xff000000);
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 8 * sizeof(Renderer::PosColorVertex)), Renderer::PosColorVertex::layout);
	}

	void Wireframe::Destroy()
	{
		if (bgfx::isValid(vbh)) bgfx::destroy(vbh);
		if (cubeVertices != nullptr)
		{
			delete[] cubeVertices;
			cubeVertices = nullptr;
		}
	}

	void Wireframe::setCoords(glm::vec3& coords)
	{
		this->world_coords = coords;
	}

	Wireframe::Wireframe()
	{}

	Wireframe::Wireframe(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::DIRT, coords, chunk_coords)
	{}

	Wireframe::~Wireframe()
	{}

	void Wireframe::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), Renderer::color_program, state);
	}
}
