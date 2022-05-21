#include "Model.h"

//    v4----- v7
//   /|      /|
//  v3------v2|
//  | |     | |
//  | |v5---|-|v6
//  |/      |/
//  v0------v1

namespace MyWorld
{
    const glm::vec3 Model::NorthFaceVec = glm::vec3{ 0.5f, 1.0f, 0.5f };
    const glm::vec3 Model::SouthFaceVec = glm::vec3{ 0.5f, 0.0f, 0.5f };
    const glm::vec3 Model::WestFaceVec = glm::vec3{ 0.0f, 0.5f, 0.5f };
    const glm::vec3 Model::EastFaceVec = glm::vec3{ 1.0f, 0.5f, 0.5f };
    const glm::vec3 Model::TopFaceVec = glm::vec3{ 0.5f, 0.5f, 1.0f };
    const glm::vec3 Model::BottomFaceVec = glm::vec3{ 0.5f, 0.5f, 0.0f };

    // used for vertex and index buffer type 2
    const int Model::faceVerticesNum = 4;
    const uint16_t Model::faceIndex[6] = {
             0,  1,  2,
             2,  3,  0,
    };

    Texture* Model::blockTexture = nullptr;
    bgfx::IndexBufferHandle Model::ibh[64];
    uint16_t* Model::triListPointers[64];
    const uint64_t Model::default_state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_BLEND_ALPHA;

    // --- Vertex and index buffer pair type 1 --- begin
    // block vertices for color
    Renderer::PosColorVertex* Model::getVerticesType0(const uint32_t color)
    {
        Renderer::PosColorVertex* vertices = new Renderer::PosColorVertex[8]{
            { 0.0f, 0.0f, 0.0f, color, }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, color, }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, color, }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f, color, }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f, color, }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f, color, }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, color, }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, color, }, // 7 --- 1,1,1
        };

        return vertices;
    }

    // three textures: side, top and bottom;
    // block vertices for texture(atlas)
    Renderer::PosTextureVertex* Model::getVerticesType1(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom)
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

        Renderer::PosTextureVertex* vertices = new Renderer::PosTextureVertex[16]{
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

    // block vertices for texture
    Renderer::PosTextureVertex* Model::getVerticesType5()
    {
        Renderer::PosTextureVertex* vertices = new Renderer::PosTextureVertex[16]{
            { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, }, // 0 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, }, // 1 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, }, // 2 --- 1,0,1
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, }, // 3 --- 0,0,1
            { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, }, // 4 --- 0,1,1
            { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, }, // 5 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, }, // 6 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, }, // 7 --- 1,1,1

            { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, }, // 0 + 8 --- 0,0,0
            { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, }, // 1 + 8 --- 1,0,0
            { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, }, // 2 + 8 --- 1,0,1 --- top
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, }, // 3 + 8 --- 0,0,1 --- top
            { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, }, // 4 + 8 --- 0,1,1 --- top
            { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, }, // 5 + 8 --- 0,1,0
            { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, }, // 6 + 8 --- 1,1,0
            { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    // block vertices for texture(atlas) color
    Renderer::PosColorTextureVertex* Model::getVerticesType2(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color)
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

        Renderer::PosColorTextureVertex* vertices = new Renderer::PosColorTextureVertex[16]{
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

    // block vetices for texture array
    Renderer::PosTextureArrayVertex* Model::getVerticesType3(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const float offset)
    {
        const float sideImageIdx = (side.y - 1) * WIDTH_NUM + side.x - 1;
        const float topImageIdx = (top.y - 1) * WIDTH_NUM + top.x - 1;
        const float bottomIdx = (bottom.y - 1) * WIDTH_NUM + bottom.x - 1;

        Renderer::PosTextureArrayVertex* vertices = new Renderer::PosTextureArrayVertex[16]{
            {       -offset,       -offset,       -offset, 0.0f, 1.0f, sideImageIdx }, // 0 --- 0,0,0
            { 1.0f + offset,       -offset,       -offset, 1.0f, 1.0f, sideImageIdx }, // 1 --- 1,0,0
            { 1.0f + offset,       -offset, 1.0f + offset, 1.0f, 0.0f, sideImageIdx }, // 2 --- 1,0,1
            {       -offset,       -offset, 1.0f + offset, 0.0f, 0.0f, sideImageIdx }, // 3 --- 0,0,1
            {       -offset, 1.0f + offset, 1.0f + offset, 1.0f, 0.0f, sideImageIdx }, // 4 --- 0,1,1
            {       -offset, 1.0f + offset,       -offset, 1.0f, 1.0f, sideImageIdx }, // 5 --- 0,1,0
            { 1.0f + offset, 1.0f + offset,       -offset, 0.0f, 1.0f, sideImageIdx }, // 6 --- 1,1,0
            { 1.0f + offset, 1.0f + offset, 1.0f + offset, 0.0f, 0.0f, sideImageIdx }, // 7 --- 1,1,1

            {       -offset,       -offset,       -offset, 1.0f, 0.0f, bottomIdx }, // 0 + 8 --- 0,0,0
            { 1.0f + offset,       -offset,       -offset, 1.0f, 1.0f, bottomIdx }, // 1 + 8 --- 1,0,0
            { 1.0f + offset,       -offset, 1.0f + offset, 1.0f, 0.0f, topImageIdx }, // 2 + 8 --- 1,0,1 --- top
            {       -offset,       -offset, 1.0f + offset, 0.0f, 0.0f, topImageIdx }, // 3 + 8 --- 0,0,1 --- top
            {       -offset, 1.0f + offset, 1.0f + offset, 0.0f, 1.0f, topImageIdx }, // 4 + 8 --- 0,1,1 --- top
            {       -offset, 1.0f + offset,       -offset, 0.0f, 0.0f, bottomIdx }, // 5 + 8 --- 0,1,0
            { 1.0f + offset, 1.0f + offset,       -offset, 0.0f, 1.0f, bottomIdx }, // 6 + 8 --- 1,1,0
            { 1.0f + offset, 1.0f + offset, 1.0f + offset, 1.0f, 1.0f, topImageIdx }  // 7 + 8 --- 1,1,1 --- top
        };

        return vertices;
    }

    // block vertices for texture array color
    Renderer::PosColorTextureArrayVertex* Model::getVerticesType4(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color)
    {
        const float sideImageIdx = (side.y - 1) * WIDTH_NUM + side.x - 1;
        const float topImageIdx = (top.y - 1) * WIDTH_NUM + top.x - 1;
        const float bottomIdx = (bottom.y - 1) * WIDTH_NUM + bottom.x - 1;

        Renderer::PosColorTextureArrayVertex* vertices = new Renderer::PosColorTextureArrayVertex[16]{
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
    void Model::createIbh(const uint8_t& idx)
    {
        const int length = 8;
        uint8_t counter = 0;
        for (uint8_t i = 0; i < 6; i++)
        {
            if (idx & (1 << i)) counter++;
        }

        uint8_t size = counter * length;

        uint16_t* cubeTriList = new uint16_t[size];

        const uint16_t cubeTriListTemplate[6 * length] =
        {
            5, 4, // North
            4, 7,
            7, 6,
            6, 5,
            0, 3, // South
            3, 2,
            2, 1,
            1, 0,
            0, 3, // West
            3, 4,
            4, 5,
            5, 0,
            1, 2, // East
            2, 7,
            7, 6,
            6, 1,
            2, 3, // Top
            3, 4,
            4, 7,
            7, 2,
            1, 0, // Bottom
            0, 5,
            5, 6,
            6, 1
        };

        uint8_t pointer = 0;
        for (uint8_t i = 0; i < 6; i++)
        {
            if (idx & (1 << i))
            {
                for (uint8_t j = 0; j < length; j++)
                {
                    cubeTriList[pointer * length + j] = cubeTriListTemplate[i * length + j];
                }
                pointer++;
            }
        }

        triListPointers[idx] = cubeTriList;
        ibh[idx] = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, size * sizeof(uint16_t)));
    }

    const bgfx::IndexBufferHandle& Model::getIbh(const uint8_t& idx)
    {
        if (ibh[idx].idx == bgfx::kInvalidHandle) createIbh(idx);
        return ibh[idx];
    }
    // --- Vertex and index buffer pair type 1 --- end

    // --- Vertex and index buffer pair type 2 --- begin
    // get vertices for faces with texture
    Renderer::PosTextureArrayVertex* Model::getFaceVertices(Block& start, Block& end, const glm::vec2& texCoord, Block::DIRECTION direction)
    {
        const float texIndex = (texCoord.y - 1) * WIDTH_NUM + texCoord.x - 1;

        const glm::vec3 startCoords = start.getCoords();
        const glm::vec3 endCoords = end.getCoords();
        const float xLength = endCoords.x - startCoords.x + 1;
        const float yLength = endCoords.y - startCoords.y + 1;
        const float zLength = endCoords.z - startCoords.z + 1;

        Renderer::PosTextureArrayVertex* vertices = new Renderer::PosTextureArrayVertex[4];

        switch (direction)
        {
        case Block::DIRECTION::NORTH:
            vertices[0] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f, startCoords.y + 1.0f,      startCoords.z,    0.0f, zLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{      startCoords.x, startCoords.y + 1.0f,      startCoords.z, xLength, zLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{      startCoords.x, startCoords.y + 1.0f, endCoords.z + 1.0f, xLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f,   endCoords.y + 1.0f, endCoords.z + 1.0f,    0.0f,    0.0f, texIndex };
            break;
        case Block::DIRECTION::SOUTH:
            vertices[0] = Renderer::PosTextureArrayVertex{      startCoords.x, startCoords.y,      startCoords.z,    0.0f, zLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f, startCoords.y,      startCoords.z, xLength, zLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f,   endCoords.y, endCoords.z + 1.0f, xLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{      startCoords.x, startCoords.y, endCoords.z + 1.0f,    0.0f,    0.0f, texIndex };
            break;
        case Block::DIRECTION::WEST:
            vertices[0] = Renderer::PosTextureArrayVertex{ startCoords.x, endCoords.y + 1.0f,      startCoords.z,    0.0f, zLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{ startCoords.x,      startCoords.y,      startCoords.z, yLength, zLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{ startCoords.x,      startCoords.y, endCoords.z + 1.0f, yLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{   endCoords.x, endCoords.y + 1.0f, endCoords.z + 1.0f,    0.0f,    0.0f, texIndex };
            break;
        case Block::DIRECTION::EAST:
            vertices[0] = Renderer::PosTextureArrayVertex{ startCoords.x + 1.0f,      startCoords.y,      startCoords.z,    0.0f, zLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{ startCoords.x + 1.0f, endCoords.y + 1.0f,      startCoords.z, yLength, zLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{   endCoords.x + 1.0f, endCoords.y + 1.0f, endCoords.z + 1.0f, yLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{ startCoords.x + 1.0f,      startCoords.y, endCoords.z + 1.0f,    0.0f,    0.0f, texIndex };
            break;
        case Block::DIRECTION::TOP:
            vertices[0] = Renderer::PosTextureArrayVertex{      startCoords.x,      startCoords.y, startCoords.z + 1.0f,    0.0f, yLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f,      startCoords.y, startCoords.z + 1.0f, xLength, yLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f, endCoords.y + 1.0f,   endCoords.z + 1.0f, xLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{      startCoords.x, endCoords.y + 1.0f, startCoords.z + 1.0f,    0.0f,    0.0f, texIndex };
            break;
        case Block::DIRECTION::BOTTOM:
            vertices[0] = Renderer::PosTextureArrayVertex{      startCoords.x, endCoords.y + 1.0f, startCoords.z,    0.0f, yLength, texIndex };
            vertices[1] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f,    endCoords.y + 1,   endCoords.z, xLength, yLength, texIndex };
            vertices[2] = Renderer::PosTextureArrayVertex{ endCoords.x + 1.0f,      startCoords.y, startCoords.z, xLength,    0.0f, texIndex };
            vertices[3] = Renderer::PosTextureArrayVertex{      startCoords.x,      startCoords.y, startCoords.z,    0.0f,    0.0f, texIndex };
            break;
        default:
            break;
        }

        return vertices;
    }
    // --- Vertex and index buffer pair type 2 --- end

    void Model::Register()
    {
        for (int i = 0; i < 64; i++)
            ibh[i] = BGFX_INVALID_HANDLE;

        blockTexture = new Texture("c:\\Bright\\Dev\\MyWorld\\resource\\images\\bin\\blocks.dds", Tools::TextureArrayParam{ WIDTH_NUM, HEIGHT_NUM });
    }

    void Model::Destroy()
    {
        for (uint8_t i = 0; i < 64; i++)
        {
            delete[] triListPointers[i];
            if (ibh[i].idx && bgfx::isValid(ibh[i])) bgfx::destroy(ibh[i]);
        }
        delete blockTexture;
        blockTexture = nullptr;
    }

    void Model::DrawStatic(bgfx::ViewId viewId, bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state, glm::vec3& coords, glm::vec3& scale)
    {
        if (!bgfx::isValid(ibh) || !bgfx::isValid(vbh)) return;

        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, coords);
        mtx = glm::scale(mtx, scale);

        // Set model matrix for rendering.
        bgfx::setTransform(&mtx);
        bgfx::setViewMode(viewId, bgfx::ViewMode::Sequential);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        // Set render states.
        bgfx::setState(state);

        // Submit primitive for rendering to view 0.
        bgfx::submit(viewId, program);
    }

    void Model::DrawTerrain(bgfx::ViewId viewId, bgfx::DynamicVertexBufferHandle& vbh, const bgfx::DynamicIndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state, glm::vec3& coords)
    {
        if (!bgfx::isValid(ibh) || !bgfx::isValid(vbh)) return;

        glm::mat4 mtx(1.0f);
        mtx = glm::translate(mtx, coords);

        // Set model matrix for rendering.
        bgfx::setTransform(&mtx);
        bgfx::setViewMode(viewId, bgfx::ViewMode::Sequential);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        blockTexture->bind();

        // Set render states.
        bgfx::setState(state);

        // Submit primitive for rendering to view 0.
        bgfx::submit(viewId, program);
    }
}
