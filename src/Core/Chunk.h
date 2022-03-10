#pragma once
#include <FastNoiseLite.h>

#include "Renderer/Camera.h"
#include "Renderer/Model/Blocks/Block.h"
#include "Renderer/Model/Blocks/Air.h"
#include "Renderer/Model/Blocks/Grass.h"
#include "Renderer/Model/Blocks/Dirt.h"
#include "Renderer/Model/Blocks/Water.h"

namespace MyWorld
{
	class Chunk
	{
	private:
		// batch terrain vertices and index
		void batchFaces(const Renderer::PosTextureArrayVertex* vertices, std::vector<Renderer::PosTextureArrayVertex>& batched_vertices, std::vector<uint16_t>& batched_index, int& count);

		// create batched terrain model (opaque blocks)
		int batching_index_type1;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type1;
		std::vector<uint16_t> batched_model_index_type1;
		bgfx::VertexBufferHandle vbh_type1;
		bgfx::IndexBufferHandle ibh_type1;

		// create batched terrain model (water)
		int batching_index_type2;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type2;
		std::vector<uint16_t> batched_model_index_type2;
		bgfx::VertexBufferHandle vbh_type2;
		bgfx::IndexBufferHandle ibh_type2;

		// greedy meshing
		void createBatchingOfFaces(Block* startBlock, Block* endBlock, Block::DIRECTION& direction);
		void greedyMergeFaces(Block::DIRECTION&& face, const int& idx);
		void faceCullingAndSeparating();
		const bool has(Block::DIRECTION face, const int& idx);
		// world chunk info
		const std::vector<Chunk*> *world_chunks;

		// the index of chunk in the world space
		const int index;
		static bool showWorldBorder;
		static FastNoiseLite noise;
		static const int CHUNK_WIDTH;
		static const int CHUNK_DEPTH;
		static const int X_OFFSET;
		static const int Y_OFFSET;
		static const int Z_OFFSET;
		static float getLength(Block* block);

		glm::vec3 coords;
		// data of blocks
		std::vector<Block*> blocks;
		// opaque and transparent blocks to be renderred
		std::vector<Block*> opaque_blocks;
		std::vector<Block*> transparent_blocks;
	public:
		static const enum Phase : uint8_t
		{
			OPAQUE_P            = 1 << 0,
			WATER_P             = 1 << 1,
			WATER_PLACEHOLDER_P = 1 << 2
		};
		// how many chunks the world has for each side
		static const int WORLD_CHUNK_NUM;
		Chunk();
		Chunk(glm::vec2 coords, int idx);
		~Chunk();
		static void toggleFaceCullingMode();
		static void Init();
		static void Destroy();
		void Draw(Phase&& phase = Phase::OPAQUE_P);
		void Build(std::vector<Chunk*>* chunks);
	};
}
