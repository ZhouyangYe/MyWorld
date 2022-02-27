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
		static bool showEdge;
		static FastNoiseLite noise;
		static const int CHUNK_WIDTH;
		static const int CHUNK_DEPTH;
		static float getLength(Block* block);
		void faceCullingAndSeparating();
		glm::vec2 coords;
		std::vector<Block*> blocks;
		std::vector<Block*> opaque_blocks;
		std::vector<Block*> transparent_blocks;
	public:
		Chunk();
		Chunk(glm::vec2 coords);
		~Chunk();
		static void toggleFaceCulling();
		static void Init();
		static void Destroy();
		void Draw();
		void toggleEdge();
	};
}
