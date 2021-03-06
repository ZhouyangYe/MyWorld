#pragma once
#include "Renderer/Camera.h"
#include "Renderer/Model/Model.h"
#include "Renderer/Model/Blocks/Air.h"
#include "Renderer/Model/Blocks/Grass.h"
#include "Renderer/Model/Blocks/Dirt.h"
#include "Renderer/Model/Blocks/Water.h"
#include "Renderer/Model/Blocks/Wireframe.h"
#include "TerrainGeneration.h"

#define X_OFFSET CHUNK_DEPTH
#define Y_OFFSET CHUNK_WIDTH * CHUNK_DEPTH
#define Z_OFFSET 1


namespace MyWorld
{
	class Chunk
	{
	private:
		struct CustomBlock
		{
			glm::vec3 coords;
			Block::TYPE type;
		};
		static glm::vec3 spawn_location;
		static float&& getLength(Block& block);
		// transparent blocks to be rendered
		static std::vector<Block> transparent_blocks;
	public:
		static const enum class Phase : uint8_t
		{
			OPAQUE_P            = 1 << 0,
			WATER_P             = 1 << 1,
			WATER_PLACEHOLDER_P = 1 << 2
		};
		static void Init();
		static void Destroy();
		static void DrawTransparent();
		static const glm::vec3& getSpawnLocation();
	private:
		glm::vec3 coords;
		// data of blocks
		std::vector<Block> blocks;

		template<typename T>
		std::vector<T>& getModelData(std::vector<T>& buffer1, std::vector<T>& buffer2)
		{
			return buffer1.size() == 0 ? buffer1 : buffer2;
		}

		// greedy meshing
		void createBatchingOfFaces(Block& startBlock, Block& endBlock, Block::DIRECTION& direction);
		void greedyMergeFaces(Block::DIRECTION&& face, const int& idx);
		void faceCullingAndSeparating();
		const bool has(Block::DIRECTION face, const int& idx);

		// batch terrain vertices and index
		void batchFaces(const Renderer::PosTextureArrayVertex* vertices, std::vector<Renderer::PosTextureArrayVertex>& batched_vertices, std::vector<uint16_t>& batched_index, int& count);

		// create batched terrain model (opaque blocks)
		int batching_index_type1;
		std::vector<Renderer::PosTextureArrayVertex>* vList_type1_current;
		std::vector<uint16_t>* iList_type1_current;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type1_one;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type1_two;
		std::vector<uint16_t> batched_model_index_type1_one;
		std::vector<uint16_t> batched_model_index_type1_two;
		bgfx::DynamicVertexBufferHandle  vbh_type1;
		bgfx::DynamicIndexBufferHandle ibh_type1;

		// create batched terrain model (water)
		int batching_index_type2;
		std::vector<Renderer::PosTextureArrayVertex>* vList_type2_current;
		std::vector<uint16_t>* iList_type2_current;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type2_one;
		std::vector<Renderer::PosTextureArrayVertex> batched_model_vertices_type2_two;
		std::vector<uint16_t> batched_model_index_type2_one;
		std::vector<uint16_t> batched_model_index_type2_two;
		bgfx::DynamicVertexBufferHandle  vbh_type2;
		bgfx::DynamicIndexBufferHandle ibh_type2;

		// custom blocks that user placed
		std::vector<CustomBlock> custom_blocks;
	public:
		const glm::vec3& getCoords();
		Chunk();
		Chunk(glm::vec2 coords);
		~Chunk();
		void Draw(Phase&& phase);
		void GenBlocks();
		void Build();
	};
}
