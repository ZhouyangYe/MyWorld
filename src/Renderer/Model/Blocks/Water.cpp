#include "Water.h"

namespace MyWorld
{
	Block::PosColorTextureVertex* Water::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Water::vbh;

	void Water::Register()
	{
		cubeVertices = Block::getVerticesType2(glm::vec2{ 1.0f, 16.0f }, glm::vec2{ 1.0f, 16.0f }, glm::vec2{ 1.0f, 16.0f }, 0xaa000000);
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosColorTextureVertex)), Renderer::getColorTextureLayout());
	}

	void Water::Destroy()
	{
		bgfx::destroy(vbh);
		delete[] cubeVertices;
		cubeVertices = nullptr;
	}

	Water::Water()
	{}

	Water::Water(glm::vec3 coords) : Block(Block::WATER, coords)
	{
	}

	Water::~Water()
	{}

	void Water::Draw(uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), Renderer::water_program, Block::default_state & (~BGFX_STATE_CULL_CW));
	}
}
