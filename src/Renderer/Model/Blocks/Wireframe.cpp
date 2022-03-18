#include "Wireframe.h"

namespace MyWorld
{
	Renderer::PosTextureArrayVertex* Wireframe::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Wireframe::vbh = BGFX_INVALID_HANDLE;
	const uint64_t Wireframe::state = Block::default_state & (~BGFX_STATE_CULL_CW);
	const glm::vec2 Wireframe::face = { 0.0f, 0.0f };
	const uint8_t Wireframe::faces = Block::DIRECTION::NORTH | Block::DIRECTION::SOUTH | Block::DIRECTION::WEST | Block::DIRECTION::EAST | Block::DIRECTION::TOP | Block::DIRECTION::BOTTOM;

	void Wireframe::Register()
	{
		cubeVertices = Block::getVerticesType3(face, face, face, 0.003);
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Renderer::PosTextureArrayVertex)), Renderer::PosTextureArrayVertex::layout);
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

	void Wireframe::setCoords(const glm::vec3& coords)
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
		Block::Draw(vbh, Block::getIbh(faces), Renderer::texture_array_program, state);
	}
}
