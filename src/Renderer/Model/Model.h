#pragma once
#include "../Renderer.h"
#include "Texture.h"
#include "Blocks/Block.h"

#define WIDTH_NUM 16
#define HEIGHT_NUM 16
#define xUnit 1/WIDTH_NUM
#define yUnit 1/HEIGHT_NUM

namespace MyWorld
{
	class Model
	{

	public:
		// the offsets of coordinate for each face of a block, used to calculate the distance of the faces to camera
		static const glm::vec3 NorthFaceVec;
		static const glm::vec3 SouthFaceVec;
		static const glm::vec3 WestFaceVec;
		static const glm::vec3 EastFaceVec;
		static const glm::vec3 TopFaceVec;
		static const glm::vec3 BottomFaceVec;

		static const uint64_t default_state;

		// creating vbh and ibh for greedy meshing
		static const int faceVerticesNum;
		static const uint16_t faceIndex[6];
		static Renderer::PosTextureArrayVertex* getFaceVertices(Block& start, Block& end, const glm::vec2& texCoord, Block::DIRECTION direction);

		static void Register();
		static void Destroy();
		// draw batched trangles created by greedy meshing
		static void DrawStatic(bgfx::ViewId viewId, bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state, glm::vec3& coords, glm::vec3& scale);
		// draw batched trangles created by greedy meshing
		static void DrawTerrain(bgfx::ViewId viewId, bgfx::DynamicVertexBufferHandle& vbh, const bgfx::DynamicIndexBufferHandle& ibh, bgfx::ProgramHandle& program, uint64_t state, glm::vec3& coords);
		// used for getting vertices for each block
		static Renderer::PosColorVertex* getVerticesType0(const uint32_t color);
		static Renderer::PosTextureVertex* getVerticesType1(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom);
		static Renderer::PosColorTextureVertex* getVerticesType2(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		static Renderer::PosTextureArrayVertex* getVerticesType3(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const float offset = 0.0f);
		static Renderer::PosColorTextureArrayVertex* getVerticesType4(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		static Renderer::PosTextureVertex* getVerticesType5();
		// get ibh when exists, if not, create it and put into cache
		static const bgfx::IndexBufferHandle& getIbh(const uint8_t& idx);
		// texture for blocks
		static Texture* blockTexture;
	private:
		// cached ibh for blocks
		static bgfx::IndexBufferHandle ibh[];
		// cached list of index for creating ibh
		static uint16_t* triListPointers[];
		// create ibh and put into cache
		static void createIbh(const uint8_t& idx);
	};
}
