#include "Block.h"

namespace MyWorld
{
    bgfx::VertexBufferHandle Block::vbh;
    bgfx::IndexBufferHandle Block::ibh;
    bgfx::UniformHandle Block::s_texColor;
    bgfx::TextureHandle Block::m_textureColor;

    Block::PosTextureVertex Block::cubeVertices[] =
    {
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // 0 --- 0,0,0
        { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // 1 --- 1,0,0
        { 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // 2 --- 1,0,1
        { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 3 --- 0,0,1
        { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 4 --- 0,1,1
        { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }, // 5 --- 0,1,0
        { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f }, // 6 --- 1,1,0
        { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 7 --- 1,1,1

        { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // 0 + 8 --- 0,0,0
        { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 1 + 8 --- 1,0,0
        { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // 2 + 8 --- 1,0,1
        { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, // 3 + 8 --- 0,0,1
        { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 4 + 8 --- 0,1,1
        { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, // 5 + 8 --- 0,1,0
        { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }, // 6 + 8 --- 1,1,0
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }  // 7 + 8 --- 1,1,1
    };

    const uint16_t Block::cubeTriList[] =
    {
         6,  5,  4, // North
         4,  7,  6,
         0,  1,  2, // South
         2,  3,  0,
         5,  0,  3, // West
         3,  4,  5,
         1,  6,  7, // East
         7,  2,  1,
        11, 10, 15, // Up
        15, 12, 11,
        13, 14,  9, // Down
         9,  8, 13
    };

    void Block::Init()
    {
        vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), Renderer::getTextureLayout());
        ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
        s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        m_textureColor = Renderer::loadTexture("c:\\Bright\\dev\\MyWorld\\resource\\images\\bin\\cute.dds");
    }

    void Block::Terminate()
    {
        bgfx::destroy(ibh);
        bgfx::destroy(vbh);
        bgfx::destroy(m_textureColor);
        bgfx::destroy(s_texColor);
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
        bgfx::setTexture(0, s_texColor, m_textureColor);

        // Set render states.
        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_MSAA
        );

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, MyWorld::Renderer::getProgramHandle());
	}
}
