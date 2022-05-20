#pragma once
#include <FastNoiseLite.h>
#include "../Util.h"
#include "Renderer/Model/Model.h"

#define CHUNK_DEPTH 386
#define CHUNK_WIDTH 16
#define TOTAL_BLOCKS CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_DEPTH

namespace MyWorld
{
	class TerrainGeneration
	{
	public:
		// get the type of the specific block
		static const Block::TYPE&& getType(glm::vec3& pos);
		static void setShowWorldBorder(bool&& show);
		static void setChunkRenderDistance(int& num);
		static void setChunkRenderDistanceNum(int& num);
		static void setChunkNum(int& num);
		static const bool& ShowWorldBorder();
		static const int& ChunkRenderDistance();
		static const int& ChunkRenderDistanceNum();
		static const int& ChunkNum();
		static void Init();
	private:
		static int chunk_num;
		static FastNoiseLite noise;
		static bool showWorldBorder;
		static int renderDistance; // SETTINGS
		// how many num of chunks to be rendered base on player
		static int renderDistanceNum;
	};
}
