#pragma once
#include "../../Renderer.h"
#include "../Texture.h"

#define xUnit 1/16
#define yUnit 1/16



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
		static PosTextureVertex* getVerticesType1(glm::vec2 &side, glm::vec2 &top, glm::vec2 &bottom);
		static bgfx::IndexBufferHandle ibh[];
	private:
		glm::vec3 coords;
		static Texture* texture;
		static bool isDebugMode;
	public:
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
			GRASS   = 3
		};
		const Block::TYPE type;
		Block();
		Block(Block::TYPE type, glm::vec3 &coords);
		static bgfx::IndexBufferHandle& getIbh(uint8_t &idx);
		static void switchRenderMode();
		static void createIbh(uint8_t& idx);
		static void Register();
		static void Destroy();
		void Draw(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh);
		virtual void Draw(uint8_t& faces) = 0;
	};
}
