#include "Block.h"

namespace MyWorld
{
    const glm::vec3& Block::getCoords()
    {
        return coords;
    }

    const glm::vec3& Block::getWorldCoords()
    {
        return world_coords;
    }

    Block::Block() : coords({ 0.0f, 0.0f, 0.0f }), type(TYPE::INVALID), chunk_id(0)
    {}

    Block::Block(Block::TYPE type, glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id) : coords(coords), world_coords(coords + glm::vec3(chunk_coords, 0)), type(type), chunk_id(chunk_id)
    {}

    Block::~Block()
    {}

	void Block::Draw(Texture* texture, bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, const uint64_t& state)
	{
        if (ibh.idx == bgfx::kInvalidHandle) return;

        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, world_coords);

        // Set model matrix for rendering.
        bgfx::setTransform(&mtx);
        bgfx::setViewMode(0, bgfx::ViewMode::Sequential);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        texture->bind();

        // Set render states.
        bgfx::setState(state);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, program);
	}
}
