#pragma once
#include "../../Renderer.h"
#include "../Texture.h"

#define WIDTH_NUM 16
#define HEIGHT_NUM 16
#define xUnit 1/WIDTH_NUM
#define yUnit 1/HEIGHT_NUM

namespace MyWorld
{
	class Block
	{
	public:
		static const enum DIRECTION : uint8_t
		{
			NORTH = 1 << 0,
			SOUTH = 1 << 1,
			WEST = 1 << 2,
			EAST = 1 << 3,
			TOP = 1 << 4,
			BOTTOM = 1 << 5
		};
		static const enum TYPE : uint64_t
		{
			INVALID = 1 << 0,
			AIR = 1 << 1,
			DIRT = 1 << 2,
			GRASS = 1 << 3,
			WATER = 1 << 4,
		};
		// the offsets of coordinate for each face of a block, used to calculate the distance of the faces to camera, useless when using greedy meshing
		static const glm::vec3 NorthFaceVec;
		static const glm::vec3 SouthFaceVec;
		static const glm::vec3 WestFaceVec;
		static const glm::vec3 EastFaceVec;
		static const glm::vec3 TopFaceVec;
		static const glm::vec3 BottomFaceVec;

		static const uint64_t default_state;
		const Block::TYPE type;
		// indicates the faces to be drawn for the block
		uint8_t faces = 0;
		Block();
		Block(Block::TYPE type, glm::vec3& coords, glm::vec2& chunk_coords);

		// creating vbh and ibh for greedy meshing
		static const int faceVerticesNum;
		static const uint16_t faceIndex[6];
		static Renderer::PosTextureArrayVertex* getFaceVertices(Block* start, Block* end, const glm::vec2& texCoord, DIRECTION direction);

		static void Register();
		static void Destroy();
		const glm::vec3& getCoords();
		const glm::vec3& getCalculatedCoords();
		// draw batched trangles created by greedy meshing
		static void DrawTerrain(bgfx::ViewId viewId, bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state, glm::vec3& coords);
		// draw faces for each block(when greedy meshing is not used)
		virtual void Draw(const uint8_t& faces) = 0;
	protected:
		// used for getting vertices for each block when greedy meshing is not used
		static Renderer::PosTextureVertex* getVerticesType1(const glm::vec2 &side, const glm::vec2 &top, const glm::vec2 &bottom);
		static Renderer::PosColorTextureVertex* getVerticesType2(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		static Renderer::PosTextureArrayVertex* getVerticesType3(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom);
		static Renderer::PosColorTextureArrayVertex* getVerticesType4(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		void Draw(bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program = Renderer::texture_program, uint64_t state = default_state);
		// coords of block, based on chunks
		glm::vec3 coords;
		// get ibh when exists, if not, create it and put into cache(when greedy meshing is not used)
		static const bgfx::IndexBufferHandle& getIbh(const uint8_t& idx);
	private:
		static Texture* texture;
		// cached ibh for blocks(when greedy meshing is not used)
		static bgfx::IndexBufferHandle ibh[];
		// cached list of index for creating ibh
		static uint16_t* triListPointers[];
		// create ibh and put into cache(when greedy meshing is not used)
		static void createIbh(const uint8_t& idx);
		// coords of block, based on the world coordinates
		glm::vec3 calculated_coords;
	};
}
