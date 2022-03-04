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
	protected:
		struct PosTextureVertex
		{
			float x;
			float y;
			float z;
			float u;
			float v;
		}; 
		struct PosTextureArrayVertex
		{
			float x;
			float y;
			float z;
			float u;
			float v;
			float d;
		};
		struct PosColorTextureVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
			float u;
			float v;
		};
		struct PosColorTextureArrayVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
			float u;
			float v;
			float d;
		};
		static PosTextureVertex* getVerticesType1(const glm::vec2 &side, const glm::vec2 &top, const glm::vec2 &bottom);
		static PosTextureArrayVertex* getVerticesType3(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom);
		static PosColorTextureVertex* getVerticesType2(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		static PosColorTextureArrayVertex* getVerticesType4(const glm::vec2& side, const glm::vec2& top, const glm::vec2& bottom, const uint32_t color);
		glm::vec3 coords;
		glm::vec2 chunk_coords;
	private:
		static Texture* texture;
		static bgfx::IndexBufferHandle ibh[];
		static uint16_t* triListPointers[];
		static void createIbh(const uint8_t& idx);
		glm::vec3 calculated_coords;
	public:
		static const glm::vec3 NorthFaceVec;
		static const glm::vec3 SouthFaceVec;
		static const glm::vec3 WestFaceVec;
		static const glm::vec3 EastFaceVec;
		static const glm::vec3 TopFaceVec;
		static const glm::vec3 BottomFaceVec;
		static const enum DIRECTION : uint8_t
		{
			NORTH  = 1 << 0,
			SOUTH  = 1 << 1,
			WEST   = 1 << 2,
			EAST   = 1 << 3,
			TOP    = 1 << 4,
			BOTTOM = 1 << 5
		};
		static const enum TYPE : uint8_t
		{
			INVALID = 0,
			AIR     = 1,
			DIRT    = 2,
			GRASS   = 3,
			WATER   = 4,
		};
		static const uint64_t default_state;
		const Block::TYPE type;
		uint8_t faces = 0;
		Block();
		Block(Block::TYPE type, glm::vec3 &coords, glm::vec2 &chunk_coords);
		static bgfx::IndexBufferHandle& getIbh(const uint8_t &idx);
		static void Register();
		static void Destroy();
		const glm::vec3& getCoords();
		const glm::vec3& getCalculatedCoords();
		void Draw(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program = Renderer::texture_program, uint64_t state = default_state);
		virtual void Draw(const uint8_t& faces) = 0;
	};
}
