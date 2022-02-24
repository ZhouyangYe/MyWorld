#pragma once
#include <FastNoiseLite.h>

#include "Renderer/Camera.h"
#include "Renderer/Model/Blocks/Block.h"
#include "Renderer/Model/Blocks/Air.h"
#include "Renderer/Model/Blocks/Grass.h"
#include "Renderer/Model/Blocks/Dirt.h"
#include "Renderer/Model/Blocks/Water.h"

#define X_OFFSET CHUNK_DEPTH
#define Y_OFFSET CHUNK_WIDTH * CHUNK_DEPTH
#define Z_OFFSET 0

namespace MyWorld
{
	class Chunk
	{
	private:
		static FastNoiseLite noise;
		static const int CHUNK_WIDTH;
		static const int CHUNK_DEPTH;
		static float getLength(Block* block);
		std::vector<Block*> blocks;
		std::vector<Block*> transparent_blocks;
	public:
		Chunk(glm::vec3 coords);
		~Chunk();
		static void Init();
		static void Terminate();
		void Draw();
	};
}
