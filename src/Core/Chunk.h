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
		// how many num of chunks the world has for each side
		static int WORLD_CHUNK_NUM;
		static bool showWorldBorder;
		static FastNoiseLite noise;
		static const int CHUNK_WIDTH;
		static const int CHUNK_DEPTH;
		static const int X_OFFSET;
		static const int Y_OFFSET;
		static const int Z_OFFSET;
		static float getLength(Block* block);
		// transparent blocks to be rendered
		static std::vector<Block*> transparent_blocks;
	public:
		static const enum Phase : uint8_t
		{
			OPAQUE_P            = 1 << 0,
			WATER_P             = 1 << 1,
			WATER_PLACEHOLDER_P = 1 << 2
		};
		static void toggleFaceCullingMode();
		static void Init();
		static void Destroy();
		static const int& getWorldChunkNum();
		static void setWorldChunkNum(int num);
		static void DrawTransparent();
	private:
		// the index of chunk in the world space
		const int index;
		glm::vec3 coords;
		// data of blocks
		std::vector<Block*> blocks;
		// opaque blocks to be renderred
		std::vector<Block*> opaque_blocks;

		// get the type of a block
		static const Block::TYPE getType(float& x, float& y, float& z);

		// greedy meshing
		void createBatchingOfFaces(Block* startBlock, Block* endBlock, Block::DIRECTION& direction);
		void greedyMergeFaces(Block::DIRECTION&& face, const int& idx);
		void faceCullingAndSeparating();
		const bool has(Block::DIRECTION face, const int& idx);

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
	public:
		Chunk();
		Chunk(glm::vec2 coords, int idx);
		~Chunk();
		void Draw(Phase&& phase);
		void Draw();
		void Build();
	};
}
