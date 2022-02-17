#include "Block.h"

namespace MyWorld
{
    Texture* Block::texture = nullptr;
    bgfx::IndexBufferHandle Block::ibh[64];
    bool Block::isDebugMode = false;

    // three textures: top, middle and bottom;
    Block::PosTextureVertex* Block::getVerticesType1(glm::vec2 &side, glm::vec2 &top, glm::vec2 &bottom)
    {
        glm::vec2 bottom_left = { (side.x - 1) * xUnit, side.y * yUnit }; // 0.0, 0.0
        glm::vec2 bottom_right = { side.x * xUnit, side.y * yUnit };      // 1.0, 0.0
        glm::vec2 top_left = { (side.x - 1) * xUnit, (side.y - 1) * yUnit };          // 0.0, 1.0
        glm::vec2 top_right = { side.x * xUnit, (side.y - 1) * yUnit };         // 1.0, 1.0

        glm::vec2 t_bottom_left = { (top.x - 1) * xUnit, top.y * yUnit }; // 0.0, 0.0
        glm::vec2 t_bottom_right = { top.x * xUnit, top.y * yUnit };      // 1.0, 0.0
        glm::vec2 t_top_left = { (top.x - 1) * xUnit, (top.y - 1) * yUnit };          // 0.0, 1.0
        glm::vec2 t_top_right = { top.x * xUnit, (top.y - 1) * yUnit };         // 1.0, 1.0

        glm::vec2 b_bottom_left = { (bottom.x - 1) * xUnit, bottom.y * yUnit }; // 0.0, 0.0
        glm::vec2 b_bottom_right = { bottom.x * xUnit, bottom.y * yUnit };      // 1.0, 0.0
        glm::vec2 b_top_left = { (bottom.x - 1) * xUnit, (bottom.y - 1) * yUnit };          // 0.0, 1.0
        glm::vec2 b_top_right = { bottom.x * xUnit, (bottom.y - 1) * yUnit };         // 1.0, 1.0

        PosTextureVertex* vertices = new PosTextureVertex[16] {
            { 0.0f, 0.0f, 0.0f,    bottom_left.x,    bottom_left.y }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f,   bottom_right.x,   bottom_right.y }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f,      top_right.x,      top_right.y }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f,       top_left.x,       top_left.y }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f,      top_right.x,      top_right.y }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f,   bottom_right.x,   bottom_right.y }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f,    bottom_left.x,    bottom_left.y }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f,       top_left.x,       top_left.y }, // 7 --- 1,1,1

            { 0.0f, 0.0f, 0.0f, b_bottom_right.x, b_bottom_right.y }, // 0 + 8 --- 0,0,0
            { 1.0f, 0.0f, 0.0f,    b_top_right.x,    b_top_right.y }, // 1 + 8 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, t_bottom_right.x, t_bottom_right.y }, // 2 + 8 --- 1,0,1 --- top
            { 0.0f, 0.0f, 1.0f,  t_bottom_left.x,  t_bottom_left.y }, // 3 + 8 --- 0,0,1 --- top
            { 0.0f, 1.0f, 1.0f,     t_top_left.x,     t_top_left.y }, // 4 + 8 --- 0,1,1 --- top
            { 0.0f, 1.0f, 0.0f,  b_bottom_left.x,  b_bottom_left.y }, // 5 + 8 --- 0,1,0
            { 1.0f, 1.0f, 0.0f,     b_top_left.x,     b_top_left.y }, // 6 + 8 --- 1,1,0
            { 1.0f, 1.0f, 1.0f,    t_top_right.x,    t_top_right.y }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    void Block::createIbh(uint8_t& idx)
    {
        uint8_t counter = 0;
        for (uint8_t i = 0; i < 6 ; i++)
        {
            if (idx & (1 << i)) counter++;
        }

        uint8_t size = counter * 6;

        uint16_t* cubeTriList = new uint16_t[size];

        const uint16_t cubeTriListTemplate[] =
        {
             6,  5,  4, // North
             4,  7,  6,
             0,  1,  2, // South
             2,  3,  0,
             5,  0,  3, // West
             3,  4,  5,
             1,  6,  7, // East
             7,  2,  1,
            11, 10, 15, // Top
            15, 12, 11,
            13, 14,  9, // Bottom
             9,  8, 13
        };

        uint8_t pointer = 0;
        for (uint8_t i = 0; i < 6; i++)
        {
            if (idx & (1 << i))
            {
                for (uint8_t j = 0; j < 6; j++)
                {
                    cubeTriList[pointer * 6 + j] = cubeTriListTemplate[i * 6 + j];
                }
                pointer++;
            }
        }

        ibh[idx] = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, size * sizeof(uint16_t)));
    }

    bgfx::IndexBufferHandle& Block::getIbh(uint8_t &idx)
    {
        if (idx == 0) return ibh[0];

        bgfx::IndexBufferHandle test = ibh[idx];

        if (!ibh[idx].idx) createIbh(idx);
        return ibh[idx];
    }

    void Block::switchRenderMode()
    {
        isDebugMode = !isDebugMode;
        if (isDebugMode)
        {
            // debug using wireframes
            bgfx::setDebug(BGFX_DEBUG_WIREFRAME);
        }
        else
        {
            bgfx::setDebug(BGFX_DEBUG_NONE);
        }
    }

    void Block::Register()
    {
        ibh[0] = BGFX_INVALID_HANDLE;

        texture = new Texture("c:\\Bright\\Dev\\MyWorld\\resource\\images\\bin\\blocks.dds", glm::vec2{ 512.0f, 1024.0f });
    }

    void Block::Destroy()
    {
        for (uint8_t i = 0; i < 64; i++)
        {
            if (ibh[i].idx && bgfx::isValid(ibh[i])) bgfx::destroy(ibh[i]);
        }
        delete texture;
        texture = nullptr;
    }

    Block::Block()
    {}

    Block::Block(glm::vec3 coords) : coords(coords)
    {
    }

	void Block::Draw(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh)
	{
        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, coords);

        // Set model matrix for rendering.
        bgfx::setTransform(&mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        texture->bind();

        // Set render states.
        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            // | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | BGFX_STATE_BLEND_ALPHA
        );

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, Renderer::getProgramHandle());
	}
}
