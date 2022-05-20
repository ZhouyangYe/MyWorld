#include "Wireframe.h"

namespace MyWorld
{
	Renderer::PosTextureVertex* Wireframe::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Wireframe::vbh = BGFX_INVALID_HANDLE;
	const uint64_t Wireframe::state = Model::default_state & (~BGFX_STATE_CULL_CW) & (~BGFX_STATE_DEPTH_TEST_LESS) | BGFX_STATE_DEPTH_TEST_LEQUAL;
	const glm::vec2 Wireframe::face = { 0.0f, 0.0f };
	const uint8_t Wireframe::faces = 
		(uint8_t)Block::DIRECTION::NORTH |
		(uint8_t)Block::DIRECTION::SOUTH |
		(uint8_t)Block::DIRECTION::WEST  |
		(uint8_t)Block::DIRECTION::EAST  |
		(uint8_t)Block::DIRECTION::TOP   |
		(uint8_t)Block::DIRECTION::BOTTOM;

	void Wireframe::Register()
	{
		cubeVertices = Model::getVerticesType5();
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Renderer::PosTextureVertex)), Renderer::PosTextureVertex::layout);
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

	Wireframe::Wireframe() : Block(Block::TYPE::WIREFRAME, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f }, 0)
	{}

	Wireframe::~Wireframe()
	{}

	void Wireframe::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Model::getIbh(faces), Renderer::outline_program, state);
	}
}
