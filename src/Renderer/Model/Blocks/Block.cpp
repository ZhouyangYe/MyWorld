#include "Block.h"

namespace MyWorld
{
    const glm::vec3 Block::NorthFaceVec  = glm::vec3{ 0.5f, 1.0f, 0.5f };
    const glm::vec3 Block::SouthFaceVec  = glm::vec3{ 0.5f, 0.0f, 0.5f };
    const glm::vec3 Block::WestFaceVec   = glm::vec3{ 0.0f, 0.5f, 0.5f };
    const glm::vec3 Block::EastFaceVec   = glm::vec3{ 1.0f, 0.5f, 0.5f };
    const glm::vec3 Block::TopFaceVec    = glm::vec3{ 0.5f, 0.5f, 1.0f };
    const glm::vec3 Block::BottomFaceVec = glm::vec3{ 0.5f, 0.5f, 0.0f };

    Texture* Block::texture = nullptr;
    bgfx::IndexBufferHandle Block::ibh[64];
    uint16_t* Block::triListPointers[64];
    const uint64_t Block::default_state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_BLEND_ALPHA;

    // --- Vertex and index buffer pair type 1 --- begin
    // three textures: side, top and bottom;
    // vertices for texture
    Block::PosTextureVertex* Block::getVerticesType1(const glm::vec2 &side, const glm::vec2 &top, const glm::vec2 &bottom)
    {
        glm::vec2 bottom_left = { (side.x - 1) * xUnit, side.y * yUnit };          // 0.0, 0.0
        glm::vec2 bottom_right = { side.x * xUnit, side.y * yUnit };               // 1.0, 0.0
        glm::vec2 top_left = { (side.x - 1) * xUnit, (side.y - 1) * yUnit };       // 0.0, 1.0
        glm::vec2 top_right = { side.x * xUnit, (side.y - 1) * yUnit };            // 1.0, 1.0

        glm::vec2 t_bottom_left = { (top.x - 1) * xUnit, top.y * yUnit };          // 0.0, 0.0
        glm::vec2 t_bottom_right = { top.x * xUnit, top.y * yUnit };               // 1.0, 0.0
        glm::vec2 t_top_left = { (top.x - 1) * xUnit, (top.y - 1) * yUnit };       // 0.0, 1.0
        glm::vec2 t_top_right = { top.x * xUnit, (top.y - 1) * yUnit };            // 1.0, 1.0

        glm::vec2 b_bottom_left = { (bottom.x - 1) * xUnit, bottom.y * yUnit };    // 0.0, 0.0
        glm::vec2 b_bottom_right = { bottom.x * xUnit, bottom.y * yUnit };         // 1.0, 0.0
        glm::vec2 b_top_left = { (bottom.x - 1) * xUnit, (bottom.y - 1) * yUnit }; // 0.0, 1.0
        glm::vec2 b_top_right = { bottom.x * xUnit, (bottom.y - 1) * yUnit };      // 1.0, 1.0

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

    // vertices for texture color
    Block::PosColorTextureVertex* Block::getVerticesType2(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color)
    {
        glm::vec2 bottom_left = { (side.x - 1) * xUnit, side.y * yUnit };          // 0.0, 0.0
        glm::vec2 bottom_right = { side.x * xUnit, side.y * yUnit };               // 1.0, 0.0
        glm::vec2 top_left = { (side.x - 1) * xUnit, (side.y - 1) * yUnit };       // 0.0, 1.0
        glm::vec2 top_right = { side.x * xUnit, (side.y - 1) * yUnit };            // 1.0, 1.0

        glm::vec2 t_bottom_left = { (top.x - 1) * xUnit, top.y * yUnit };          // 0.0, 0.0
        glm::vec2 t_bottom_right = { top.x * xUnit, top.y * yUnit };               // 1.0, 0.0
        glm::vec2 t_top_left = { (top.x - 1) * xUnit, (top.y - 1) * yUnit };       // 0.0, 1.0
        glm::vec2 t_top_right = { top.x * xUnit, (top.y - 1) * yUnit };            // 1.0, 1.0

        glm::vec2 b_bottom_left = { (bottom.x - 1) * xUnit, bottom.y * yUnit };    // 0.0, 0.0
        glm::vec2 b_bottom_right = { bottom.x * xUnit, bottom.y * yUnit };         // 1.0, 0.0
        glm::vec2 b_top_left = { (bottom.x - 1) * xUnit, (bottom.y - 1) * yUnit }; // 0.0, 1.0
        glm::vec2 b_top_right = { bottom.x * xUnit, (bottom.y - 1) * yUnit };      // 1.0, 1.0

        PosColorTextureVertex* vertices = new PosColorTextureVertex[16]{
            { 0.0f, 0.0f, 0.0f, color,    bottom_left.x,    bottom_left.y }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, color,   bottom_right.x,   bottom_right.y }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, color,      top_right.x,      top_right.y }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f, color,       top_left.x,       top_left.y }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f, color,      top_right.x,      top_right.y }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f, color,   bottom_right.x,   bottom_right.y }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, color,    bottom_left.x,    bottom_left.y }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, color,       top_left.x,       top_left.y }, // 7 --- 1,1,1

            { 0.0f, 0.0f, 0.0f, color, b_bottom_right.x, b_bottom_right.y }, // 0 + 8 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, color,    b_top_right.x,    b_top_right.y }, // 1 + 8 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, color, t_bottom_right.x, t_bottom_right.y }, // 2 + 8 --- 1,0,1 --- top
            { 0.0f, 0.0f, 1.0f, color,  t_bottom_left.x,  t_bottom_left.y }, // 3 + 8 --- 0,0,1 --- top
            { 0.0f, 1.0f, 1.0f, color,     t_top_left.x,     t_top_left.y }, // 4 + 8 --- 0,1,1 --- top
            { 0.0f, 1.0f, 0.0f, color,  b_bottom_left.x,  b_bottom_left.y }, // 5 + 8 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, color,     b_top_left.x,     b_top_left.y }, // 6 + 8 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, color,    t_top_right.x,    t_top_right.y }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    // vetices for texture array
    Block::PosTextureArrayVertex* Block::getVerticesType3(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom)
    {
        const float sideImageIdx = (side.y - 1) * WIDTH_NUM + side.x - 1;
        const float topImageIdx = (top.y - 1) * WIDTH_NUM + top.x - 1;
        const float bottomIdx = (bottom.y - 1) * WIDTH_NUM + bottom.x - 1;

        PosTextureArrayVertex* vertices = new PosTextureArrayVertex[16]{
            { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, sideImageIdx }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, sideImageIdx }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, sideImageIdx }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sideImageIdx }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, sideImageIdx }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, sideImageIdx }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, sideImageIdx }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, sideImageIdx }, // 7 --- 1,1,1

            { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, bottomIdx }, // 0 + 8 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, bottomIdx }, // 1 + 8 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, topImageIdx }, // 2 + 8 --- 1,0,1 --- top
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, topImageIdx }, // 3 + 8 --- 0,0,1 --- top
            { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, topImageIdx }, // 4 + 8 --- 0,1,1 --- top
            { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, bottomIdx }, // 5 + 8 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, bottomIdx }, // 6 + 8 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, topImageIdx }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    // vertices for texture array color
    Block::PosColorTextureArrayVertex* Block::getVerticesType4(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color)
    {
        const float sideImageIdx = (side.y - 1) * WIDTH_NUM + side.x - 1;
        const float topImageIdx = (top.y - 1) * WIDTH_NUM + top.x - 1;
        const float bottomIdx = (bottom.y - 1) * WIDTH_NUM + bottom.x - 1;

        PosColorTextureArrayVertex* vertices = new PosColorTextureArrayVertex[16]{
            { 0.0f, 0.0f, 0.0f, color, 0.0f, 1.0f, sideImageIdx }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, color, 1.0f, 1.0f, sideImageIdx }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, color, 1.0f, 0.0f, sideImageIdx }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f, color, 0.0f, 0.0f, sideImageIdx }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f, color, 1.0f, 0.0f, sideImageIdx }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f, color, 1.0f, 1.0f, sideImageIdx }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, color, 0.0f, 1.0f, sideImageIdx }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, color, 0.0f, 0.0f, sideImageIdx }, // 7 --- 1,1,1

            { 0.0f, 0.0f, 0.0f, color, 1.0f, 0.0f, bottomIdx }, // 0 + 8 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, color, 1.0f, 1.0f, bottomIdx }, // 1 + 8 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, color, 1.0f, 0.0f, topImageIdx }, // 2 + 8 --- 1,0,1 --- top
            { 0.0f, 0.0f, 1.0f, color, 0.0f, 0.0f, topImageIdx }, // 3 + 8 --- 0,0,1 --- top
            { 0.0f, 1.0f, 1.0f, color, 0.0f, 1.0f, topImageIdx }, // 4 + 8 --- 0,1,1 --- top
            { 0.0f, 1.0f, 0.0f, color, 0.0f, 0.0f, bottomIdx }, // 5 + 8 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, color, 0.0f, 1.0f, bottomIdx }, // 6 + 8 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, color, 1.0f, 1.0f, topImageIdx }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    // create cache for static index buffer
    void Block::createIbh(const uint8_t& idx)
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

        triListPointers[idx] = cubeTriList;
        ibh[idx] = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, size * sizeof(uint16_t)));
    }

    bgfx::IndexBufferHandle& Block::getIbh(const uint8_t &idx)
    {
        if (idx == 0) return ibh[0];

        if (ibh[idx].idx == bgfx::kInvalidHandle) createIbh(idx);
        return ibh[idx];
    }
    // --- Vertex and index buffer pair type 1 --- end

    const glm::vec3& Block::getCoords()
    {
        return coords;
    }

    void Block::Register()
    {
        for (int i = 0; i < 64; i++)
            ibh[i] = BGFX_INVALID_HANDLE;

        texture = new Texture("c:\\Bright\\Dev\\MyWorld\\resource\\images\\bin\\blocks.dds", Tools::TextureArrayParam{ WIDTH_NUM, HEIGHT_NUM });
    }

    void Block::Destroy()
    {
        for (uint8_t i = 0; i < 64; i++)
        {
            delete[] triListPointers[i];
            if (ibh[i].idx && bgfx::isValid(ibh[i])) bgfx::destroy(ibh[i]);
        }
        delete texture;
        texture = nullptr;
    }

    Block::Block() : coords({ 0.0f, 0.0f, 0.0f }), type(TYPE::INVALID)
    {}

    Block::Block(Block::TYPE type, glm::vec3 &coords) : coords(coords), type(type)
    {
    }

	void Block::Draw(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state)
	{
        if (ibh.idx == bgfx::kInvalidHandle) return;

        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, coords);

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
