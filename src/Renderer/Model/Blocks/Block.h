#pragma once
#include "../../Renderer.h"
#include "../Texture.h"

namespace MyWorld
{
	class Block
	{
	public:
		static const enum class DIRECTION : uint8_t
		{
			NORTH = 1 << 0,
			SOUTH = 1 << 1,
			WEST = 1 << 2,
			EAST = 1 << 3,
			TOP = 1 << 4,
			BOTTOM = 1 << 5,
			INVALID = 1 << 6
		};
		static const enum class TYPE : uint8_t
		{
			INVALID = 0,
			AIR = 1,
			DIRT = 2,
			GRASS = 3,
			WATER = 4,
			WIREFRAME = 5
		};
		const Block::TYPE type;
		// indicates which chunk it belongs to(in order to remove it from the chunk when destroyed)
		const int chunk_id;
		// indicates the faces to be drawn for the block
		uint8_t faces = 0;
		Block();
		Block(Block::TYPE type, glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id = 0);
		~Block();
		const glm::vec3& getCoords();
		const glm::vec3& getWorldCoords();
	protected:
		// coords of block, based on chunks
		glm::vec3 coords;
		void Draw(Texture* texture, bgfx::VertexBufferHandle& vbh, const bgfx::IndexBufferHandle& ibh, bgfx::ProgramHandle& program, const uint64_t& state);
		// coords of block, based on the world coordinates
		glm::vec3 world_coords;
	};
}
