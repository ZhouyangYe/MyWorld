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
#define Z_OFFSET 1

namespace MyWorld
{
	class Chunk
	{
	private:
		// create batched terrain model (opaque blocks)
		int batching_index_type1;
		std::vector<Block::PosTextureArrayVertex> batched_model_vertices_type1;
		std::vector<uint16_t> batched_model_index_type1;
		bgfx::VertexBufferHandle vbh_type1;
		bgfx::IndexBufferHandle ibh_type1;
		bgfx::ProgramHandle program_type1;
		void batchFacesForType1(const Block::PosTextureArrayVertex* vertices);

		// create batched terrain model (transparent blocks)
		int batching_index_type2;
		std::vector<Block::PosColorTextureArrayVertex> batched_model_vertices_type2;
		std::vector<uint16_t> batched_model_index_type2;
		bgfx::VertexBufferHandle vbh_type2;
		bgfx::IndexBufferHandle ibh_type2;
		bgfx::ProgramHandle program_type2;
		void batchFacesForType2(const Block::PosColorTextureArrayVertex* vertices);

		// greedy meshing
		void createBatchingOfFaces(Block* startBlock, Block* endBlock, Block::DIRECTION direction);
		void greedyMergeFaces(Block::DIRECTION face, const int& idx);
		void faceCullingAndSeparating();
		const bool has(Block::DIRECTION face, const int& idx);

		static bool showEdge;
		static FastNoiseLite noise;
		static const int CHUNK_WIDTH;
		static const int CHUNK_DEPTH;
		static float getLength(Block* block);

		glm::vec3 coords;
		std::vector<Block*> blocks;
		// opaque and transparent blocks to be renderred
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
