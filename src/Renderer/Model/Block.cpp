#include "Block.h"

namespace MyWorld
{
    bgfx::VertexBufferHandle Block::vbh;
    bgfx::IndexBufferHandle Block::ibh;

    Block::PosColorVertex Block::cubeVertices[] =
    {
        { 0.0f, 0.0f, 0.0f, 0xff000000 }, // 0 - 0,0,0
        { 1.0f, 0.0f, 0.0f, 0xff0000ff }, // 1 - 1,0,0
        { 1.0f, 0.0f, 1.0f, 0xff00ff00 }, // 2 - 1,0,1
        { 0.0f, 0.0f, 1.0f, 0xff00ffff }, // 3 - 0,0,1
        { 0.0f, 1.0f, 1.0f, 0xffff0000 }, // 4 - 0,1,1
        { 0.0f, 1.0f, 0.0f, 0xffff00ff }, // 5 - 0,1,0
        { 1.0f, 1.0f, 0.0f, 0xffffff00 }, // 6 - 1,1,0
        { 1.0f, 1.0f, 1.0f, 0xffffffff }  // 7 - 1,1,1
    };

    const uint16_t Block::cubeTriList[] =
    {
        6, 5, 4, // North
        4, 7, 6,
        0, 1, 2, // South
        2, 3, 0,
        5, 0, 3, // West
        3, 4, 5,
        1, 6, 7, // East
        7, 2, 1,
        3, 2, 7, // Up
        7, 4, 3,
        5, 6, 1, // Down
        1, 0, 5
    };

    void Block::Init()
    {
        vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), Renderer::getColorLayout());
        ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
    }

    void Block::Terminate()
    {
        bgfx::destroy(ibh);
        bgfx::destroy(vbh);
    }

    Block::Block()
    {}

    Block::Block(glm::vec3 coords) : coords(coords)
    {
    }

	void Block::Draw()
	{
        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, coords);

        // Set model matrix for rendering.
        bgfx::setTransform(&mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, MyWorld::Renderer::getProgramHandle());
	}
}
