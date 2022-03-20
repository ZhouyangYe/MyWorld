#include "Water.h"

namespace MyWorld
{
	const glm::vec2 Water::face = { 1.0f, 16.0f };
	const uint64_t Water::state = Model::default_state & (~BGFX_STATE_CULL_CW) & (~BGFX_STATE_DEPTH_TEST_LESS) | BGFX_STATE_DEPTH_TEST_LEQUAL;
	const uint64_t Water::placeholder_state = BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS;

	const Renderer::PosTextureArrayVertex* Water::getFaceVertices(Block& start, Block& end, Block::DIRECTION direction)
	{
		return Model::getFaceVertices(start, end, face, direction);
	}

	void Water::Register()
	{}

	void Water::Destroy()
	{}

	Water::Water()
	{}

	Water::Water(glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id) : Block(Block::TYPE::WATER, coords, chunk_coords, chunk_id)
	{}

	Water::~Water()
	{}
}
