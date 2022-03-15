#include "Chunk.h"

namespace MyWorld
{
	// numbers of chunks to be rendered based on the player
	int Chunk::WORLD_CHUNK_RENDER_DISTANCE = 8;
	glm::vec3 Chunk::spawn_location{ 0.0f, 0.0f, -1.0f };
	// CHUNK_DEPTH blocks high and CHUNK_WIDTH blocks wide
	const int Chunk::CHUNK_DEPTH = 386;
	const int Chunk::CHUNK_WIDTH = 16;

	const int Chunk::X_OFFSET = CHUNK_DEPTH;
	const int Chunk::Y_OFFSET = CHUNK_WIDTH * CHUNK_DEPTH;
	const int Chunk::Z_OFFSET = 1;

	bool Chunk::showWorldBorder = true;
	FastNoiseLite Chunk::noise;
	std::vector<Block*> Chunk::transparent_blocks;

	const glm::vec3& Chunk::getSpawnLocation()
	{
		return spawn_location;
	}

	// get unit blocks' faces' distance to camera(when greedy meshing is not used)
	float Chunk::getLength(Block* block)
	{
		const glm::vec3 blockCoords = block->getWorldCoords();
		glm::vec3 center;
		switch (block->faces)
		{
		case Block::DIRECTION::NORTH:
			center = blockCoords + Block::NorthFaceVec;
			break;
		case Block::DIRECTION::SOUTH:
			center = blockCoords + Block::SouthFaceVec;
			break;
		case Block::DIRECTION::WEST:
			center = blockCoords + Block::WestFaceVec;
			break;
		case Block::DIRECTION::EAST:
			center = blockCoords + Block::EastFaceVec;
			break;
		case Block::DIRECTION::TOP:
			center = blockCoords + Block::TopFaceVec;
			break;
		case Block::DIRECTION::BOTTOM:
			center = blockCoords + Block::BottomFaceVec;
			break;
		default:
			break;
		}

		return glm::length2(center - Camera::getCameraCoords());
	}

	// check if we should show the face of certain direction for the block
	const bool Chunk::has(Block::DIRECTION face, const int& idx)
	{
		Block* block = blocks[idx];
		Block::TYPE type = block->type;
		glm::vec3 blockCoords = block->getCoords();

		bool isChunkBorder = false;
		bool isWorldBorder = false;
		std::function<bool()> adjacentIsAir;
		std::function<bool()> adjacentIsWater;

		const int chunkNum = WORLD_CHUNK_RENDER_DISTANCE * 2;
		bool noAdjacent = false;
		float offset = 0.0f;
		Block::TYPE adjacentType = Block::INVALID;

		switch (face)
		{
		case Block::NORTH:
			noAdjacent = index >= chunkNum * (chunkNum - 1);
			isChunkBorder = blockCoords.y == (CHUNK_WIDTH - 1);
			isWorldBorder = isChunkBorder && noAdjacent;
			if (!isWorldBorder)
			{
				offset = blockCoords.y + coords.y + 1.0f;
				float worldX = blockCoords.x + coords.x;
				adjacentType = getType(worldX, offset, blockCoords.z);
			}
			adjacentIsAir = [&] {
				return
					(isChunkBorder && adjacentType == Block::AIR) ||
					(!isChunkBorder && blocks[idx + Y_OFFSET]->type == Block::AIR);
			};
			adjacentIsWater = [&] {
				return 
					type != Block::WATER && ((!isChunkBorder && blocks[idx + Y_OFFSET]->type == Block::WATER) ||
					(isChunkBorder && adjacentType == Block::WATER));
			};
			return (type == Block::WATER || showWorldBorder || !isWorldBorder) && (isWorldBorder || adjacentIsAir() || adjacentIsWater());
		case Block::SOUTH:
			noAdjacent = index < chunkNum;
			isChunkBorder = blockCoords.y == 0;
			isWorldBorder = isChunkBorder && noAdjacent;
			if (!isWorldBorder)
			{
				offset = blockCoords.y + coords.y + - 1.0f;
				float worldX = blockCoords.x + coords.x;
				adjacentType = getType(worldX, offset, blockCoords.z);
			}
			adjacentIsAir = [&] {
				return 
					(isChunkBorder && adjacentType == Block::AIR) ||
					(!isChunkBorder && blocks[idx - Y_OFFSET]->type == Block::AIR);
			};
			adjacentIsWater = [&] {
				return 
					type != Block::WATER && ((!isChunkBorder && blocks[idx - Y_OFFSET]->type == Block::WATER) ||
					(isChunkBorder && adjacentType == Block::WATER));
			};
			return (type == Block::WATER || showWorldBorder || !isWorldBorder) && (isWorldBorder || adjacentIsAir() || adjacentIsWater());
		case Block::WEST:
			noAdjacent = index % chunkNum == 0;
			isChunkBorder = blockCoords.x == 0;
			isWorldBorder = isChunkBorder && noAdjacent;
			if (!isWorldBorder)
			{
				offset = blockCoords.x + coords.x - 1.0f;
				float worldY = blockCoords.y + coords.y;
				adjacentType = getType(offset, worldY, blockCoords.z);
			}
			adjacentIsAir = [&] {
				return 
					(isChunkBorder && adjacentType == Block::AIR) ||
					(!isChunkBorder && blocks[idx - X_OFFSET]->type == Block::AIR);
			};
			adjacentIsWater = [&] {
				return 
					type != Block::WATER && ((!isChunkBorder && blocks[idx - X_OFFSET]->type == Block::WATER) ||
					(isChunkBorder && adjacentType == Block::WATER));
			};
			return (type == Block::WATER || showWorldBorder || !isWorldBorder) && (isWorldBorder || adjacentIsAir() || adjacentIsWater());
		case Block::EAST:
			noAdjacent = index % chunkNum == chunkNum - 1;
			isChunkBorder = blockCoords.x == CHUNK_WIDTH - 1;
			isWorldBorder = isChunkBorder && noAdjacent;
			if (!isWorldBorder)
			{
				offset = blockCoords.x + coords.x + 1.0f;
				float worldY = blockCoords.y + coords.y;
				adjacentType = getType(offset, worldY, blockCoords.z);
			}
			adjacentIsAir = [&] {
				return 
					(isChunkBorder && adjacentType == Block::AIR) ||
					(!isChunkBorder && blocks[idx + X_OFFSET]->type == Block::AIR);
			};
			adjacentIsWater = [&] {
				return 
					type != Block::WATER && ((!isChunkBorder && blocks[idx + X_OFFSET]->type == Block::WATER) ||
					(isChunkBorder && adjacentType == Block::WATER));
			};
			return (type == Block::WATER || showWorldBorder || !isWorldBorder) && (isWorldBorder || adjacentIsAir() || adjacentIsWater());
		case Block::TOP:
			isWorldBorder = blockCoords.z == CHUNK_DEPTH - 1;
			adjacentIsAir = [&] { return blocks[idx + Z_OFFSET]->type == Block::AIR; };
			adjacentIsWater = [&] { return type != Block::WATER && blocks[idx + Z_OFFSET]->type == Block::WATER; };
			return isWorldBorder || adjacentIsAir() || adjacentIsWater();
		case Block::BOTTOM:
			isWorldBorder = blockCoords.z == 0;
			adjacentIsAir = [&] { return blocks[idx - Z_OFFSET]->type == Block::AIR; };
			adjacentIsWater = [&] { return type != Block::WATER && blocks[idx - Z_OFFSET]->type == Block::WATER; };
			return (isWorldBorder || adjacentIsAir() || adjacentIsWater()) && (showWorldBorder || !isChunkBorder);
		default:
			return false;
		}
	}

	// merge vertices of each face into one array
	void Chunk::batchFaces(const Renderer::PosTextureArrayVertex* vertices, std::vector<Renderer::PosTextureArrayVertex>& batched_vertices, std::vector<uint16_t>& batched_index, int& count)
	{
		for (int i = 0; i < Block::faceVerticesNum; i++)
		{
			batched_vertices.push_back(vertices[i]);
		}

		for (int i = 0; i < Block::faceVerticesNum + 2; i++)
		{
			batched_index.push_back(Block::faceIndex[i] + count * Block::faceVerticesNum);
		}

		delete[] vertices;

		count++;
	}

	void Chunk::createBatchingOfFaces(Block* startBlock, Block* endBlock, Block::DIRECTION& direction)
	{
		switch (startBlock->type)
		{
		case Block::DIRT:
		{
			const Renderer::PosTextureArrayVertex* vertices = Dirt::getFaceVertices(startBlock, endBlock, direction);
			batchFaces(vertices, vList_type1_current, iList_type1_current, batching_index_type1);
			break;
		}
		case Block::GRASS:
		{
			const Renderer::PosTextureArrayVertex* vertices = Grass::getFaceVertices(startBlock, endBlock, direction);
			batchFaces(vertices, vList_type1_current, iList_type1_current, batching_index_type1);
			break;
		}
		case Block::WATER:
		{
			const Renderer::PosTextureArrayVertex* vertices = Water::getFaceVertices(startBlock, endBlock, direction);
			batchFaces(vertices, vList_type2_current, iList_type2_current, batching_index_type2);
			break;
		}
		default:
			break;
		}
	}

	// greedy meshing for faces(main logic)
	void Chunk::greedyMergeFaces(Block::DIRECTION&& face, const int& idx)
	{
		Block* block = blocks[idx];
		const glm::vec3 blockCoords = block->getCoords();

		int xOffset = 0, yOffset = 0, zOffset = 0, xCount = 0, yCount = 0, zCount = 0, xLength = 0, yLength = 0;

		switch (face)
		{
		case Block::NORTH:
		case Block::SOUTH:
			xOffset = X_OFFSET;
			yOffset = Z_OFFSET;
			zOffset = Y_OFFSET;
			xCount = (int)blockCoords.x + 1;
			yCount = (int)blockCoords.z;
			zCount = (int)blockCoords.y;
			xLength = CHUNK_WIDTH;
			yLength = CHUNK_DEPTH;
			break;
		case Block::WEST:
		case Block::EAST:
			xOffset = Y_OFFSET;
			yOffset = Z_OFFSET;
			zOffset = X_OFFSET;
			xCount = (int)blockCoords.y + 1;
			yCount = (int)blockCoords.z;
			zCount = (int)blockCoords.x;
			xLength = CHUNK_WIDTH;
			yLength = CHUNK_DEPTH;
			break;
		case Block::TOP:
		case Block::BOTTOM:
			xOffset = X_OFFSET;
			yOffset = Y_OFFSET;
			zOffset = Z_OFFSET;
			xCount = (int)blockCoords.x + 1;
			yCount = (int)blockCoords.y;
			zCount = (int)blockCoords.z;
			xLength = CHUNK_WIDTH;
			yLength = CHUNK_WIDTH;
			break;
		default:
			break;
		}

		block->faces |= face;
		const int startingXCount = xCount - 1;
		const int startingYCount = yCount;
		int xIndex = zCount * zOffset + yCount * yOffset + xCount * xOffset;
		int hitEdge = false;
		while (yCount < yLength && !hitEdge)
		{
			if (yCount == startingYCount && xCount < xLength && (blocks[xIndex]->faces & face) == 0 && blocks[xIndex]->type == block->type && has(face, xIndex))
			{
				blocks[xIndex]->faces |= face;
				xCount++;
				xIndex = zCount * zOffset + yCount * yOffset + xCount * xOffset;
			}
			else
			{
				if (yCount == startingYCount)
				{
					yCount++;
					if (yCount == yLength)
					{
						// hitEdge = true; // not necessary?
						break;
					}
				}
				for (int i = startingXCount; i < xCount; i++)
				{
					const int yIndex = zCount * zOffset + i * xOffset + yCount * yOffset;
					if ((blocks[yIndex]->faces & face) == 0 && blocks[yIndex]->type == block->type && has(face, yIndex))
					{
						blocks[yIndex]->faces |= face;
					}
					else
					{
						if (i > startingXCount)
						{
							for (int j = i - 1; j >= startingXCount; j--)
							{
								blocks[zCount * zOffset + j * xOffset + yCount * yOffset]->faces &= ~face;
							}
						}
						hitEdge = true;
						break;
					}
				}
				if (!hitEdge) yCount++;
			}
		}

		createBatchingOfFaces(block, blocks[zCount * zOffset + (xCount - 1) * xOffset + (yCount - 1) * yOffset], face);
	}

	// figure out which faces to be drawn, merge faces, and put opaque and transparent blocks into different arrays
	void Chunk::faceCullingAndSeparating()
	{
		// TODO: 1. frustum culling 2. do this in a separate thread
		for (int i = 0; i < blocks.size(); i++)
		{
			if (blocks[i]->type != Block::AIR)
			{
				Block* block = blocks[i];
				Block::TYPE type = block->type;

				if (has(Block::DIRECTION::BOTTOM, i))
				{
					if ((block->faces & Block::DIRECTION::BOTTOM) == 0) greedyMergeFaces(Block::DIRECTION::BOTTOM, i);
				}
				if (has(Block::DIRECTION::TOP, i))
				{
					if ((block->faces & Block::DIRECTION::TOP) == 0) greedyMergeFaces(Block::DIRECTION::TOP, i);
				}

				if (has(Block::DIRECTION::WEST, i))
				{
					if ((block->faces & Block::DIRECTION::WEST) == 0) greedyMergeFaces(Block::DIRECTION::WEST, i);
				}
				if (has(Block::DIRECTION::EAST, i))
				{
					if ((block->faces & Block::DIRECTION::EAST) == 0) greedyMergeFaces(Block::DIRECTION::EAST, i);
				}

				if (has(Block::DIRECTION::SOUTH, i))
				{
					if ((block->faces & Block::DIRECTION::SOUTH) == 0) greedyMergeFaces(Block::DIRECTION::SOUTH, i);
				}
				if (has(Block::DIRECTION::NORTH, i))
				{
					if ((block->faces & Block::DIRECTION::NORTH) == 0) greedyMergeFaces(Block::DIRECTION::NORTH, i);
				}
			}
		}
	}

	void Chunk::toggleFaceCullingMode()
	{
		showWorldBorder = !showWorldBorder;
	}

	void Chunk::Init()
	{
		noise.SetSeed(666);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.003f);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFractalOctaves(6);
		noise.SetFractalLacunarity(1.5f);
		noise.SetFractalGain(1.4f);
		noise.SetFractalWeightedStrength(0.4f);
		// noise.SetFractalPingPongStrength(1.5f);
	}

	void Chunk::Destroy()
	{}

	Chunk::Chunk() : index(0)
	{}

	Chunk::Chunk(glm::vec2 coords, int index) :
		index(index),
		coords(glm::vec3(coords, 0.0f)), 
		vbh_type1(BGFX_INVALID_HANDLE),
		ibh_type1(BGFX_INVALID_HANDLE),
		vbh_type2(BGFX_INVALID_HANDLE),
		ibh_type2(BGFX_INVALID_HANDLE),
		batching_index_type1(0),
		batching_index_type2(0)
	{
		GenBlocks();
		Build();

		// create model buffer for the chunk terrain
		// opaque
		if (vList_type1_current.size())
		{
		  vbh_type1 = bgfx::createDynamicVertexBuffer(bgfx::makeRef(vList_type1_current.data(), vList_type1_current.size() * sizeof(Renderer::PosTextureArrayVertex)), Renderer::PosTextureArrayVertex::layout);
		  ibh_type1 = bgfx::createDynamicIndexBuffer(bgfx::makeRef(iList_type1_current.data(), iList_type1_current.size() * sizeof(uint16_t)));
		}

		// water
		if (vList_type2_current.size())
		{
		  vbh_type2 = bgfx::createDynamicVertexBuffer(bgfx::makeRef(vList_type2_current.data(), vList_type2_current.size() * sizeof(Renderer::PosTextureArrayVertex)), Renderer::PosTextureArrayVertex::layout);
		  ibh_type2 = bgfx::createDynamicIndexBuffer(bgfx::makeRef(iList_type2_current.data(), iList_type2_current.size() * sizeof(uint16_t)));
		}
	}

	void Chunk::GenBlocks()
	{
		// create all blocks in the chunk
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				for (int z = 0; z < CHUNK_DEPTH; z++)
				{
					float worldX = x + coords.x, wolrdY = y + coords.y, worldZ = z;
					const Block::TYPE type = getType(worldX, wolrdY, worldZ);

					// set spawn location
					if (spawn_location.z == -1.0f && type == Block::AIR && coords.x == 0.0f && coords.y == 0.0f)
					{
						spawn_location = glm::vec3{ 0.0f, 0.0f, z + 3.0f };
					}

					switch (type)
					{
					case Block::AIR:
						blocks.push_back(new Air({ x, y, z }, coords));
						break;
					case Block::WATER:
						blocks.push_back(new Water({ x, y, z }, coords, index));
						break;
					case Block::DIRT:
						blocks.push_back(new Dirt({ x, y, z }, coords));
						break;
					case Block::GRASS:
						blocks.push_back(new Grass({ x, y, z }, coords));
						break;
					default:
						break;
					}
				}
			}
		}
	}

	void Chunk::Build()
	{
		// switch working data to be calculated
		vList_type1_current = getModelData(batched_model_vertices_type1_one, batched_model_vertices_type1_two);
		iList_type1_current = getModelData(batched_model_index_type1_one, batched_model_index_type1_two);
		vList_type2_current = getModelData(batched_model_vertices_type2_one, batched_model_vertices_type2_two);
		iList_type2_current = getModelData(batched_model_index_type2_one, batched_model_index_type2_two);

		faceCullingAndSeparating();

		// remove blocks when chunk builds are done
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		{
		  delete (*iter);
		}
		blocks.clear();
	}

	// TODO: implement more interesting terrain
	const Block::TYPE Chunk::getType(float& x, float& y, float& z)
	{
		const float data = (noise.GetNoise(x, y) + 1) / 2;
		const int surface = (int)(CHUNK_DEPTH * data);
		const int edge = surface - 1;
		const int waterLine = 190;

		if (z < surface)
		{
			if (z == edge && z >= waterLine)
			{
				return Block::GRASS;
			}
			else
			{
				return Block::DIRT;
			}
		}
		else if (z <= waterLine)
		{
			return Block::WATER;
		}
		else
		{
			return Block::AIR;
		}
	}

	Chunk::~Chunk()
	{
		// remove transparent blocks data for this chunk
		std::vector<Block*> temp = transparent_blocks;
		transparent_blocks.clear();
		for (std::vector<Block*>::iterator iter = temp.begin(); iter != temp.end(); ++iter)
		{
			if ((*iter)->type == Block::WATER && ((Water*)(*iter))->chunk_id == index)
			{
				delete (*iter);
				continue;
			}
			transparent_blocks.push_back(*iter);
		}

		// destroy data(no greedy meshing)
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		{
			delete (*iter);
		}

		// destroy data(with greedy meshing)
		// opaque
		if (bgfx::isValid(vbh_type1)) bgfx::destroy(vbh_type1);
		if (bgfx::isValid(ibh_type1)) bgfx::destroy(ibh_type1);
		// water
		if (bgfx::isValid(vbh_type2)) bgfx::destroy(vbh_type2);
		if (bgfx::isValid(ibh_type2)) bgfx::destroy(ibh_type2);
	}

	// draw transparent blocks
	void Chunk::DrawTransparent()
	{
		int size = transparent_blocks.size();
		if (size)
		{
			Block** sortedBlocks = transparent_blocks.data();
			// draw far faces first
			mergeSort<Block*>(sortedBlocks, size, [](Block* item1, Block* item2) {
				return getLength(item1) - getLength(item2);
			});
			for (int i = 0; i < size; i++)
			{
				sortedBlocks[i]->Draw(sortedBlocks[i]->faces);
			}
		}
	}

	// draw batched terrain
	void Chunk::Draw(Phase&& phase)
	{
		switch (phase)
		{
		case Phase::OPAQUE_P:
			// draw opaque terrain
			Block::DrawTerrain(Tools::DEFAULT_VIEW_ID, vbh_type1, ibh_type1, Renderer::texture_array_program, Block::default_state, coords);
			break;
		case Phase::WATER_PLACEHOLDER_P:
			// draw water placeholder(depth buffer)
			Block::DrawTerrain(Tools::DEFAULT_VIEW_ID, vbh_type2, ibh_type2, Renderer::water_program, Water::placeholder_state, coords);
			break;
		case Phase::WATER_P:
			// draw water
			Block::DrawTerrain(Tools::DEFAULT_VIEW_ID, vbh_type2, ibh_type2, Renderer::water_program, Water::state, coords);
			break;
		default:
			break;
		}
	}

	const int& Chunk::getChunkRenderDistanceNum()
	{
		return WORLD_CHUNK_RENDER_DISTANCE;
	}

	void Chunk::setChunkRenderDistanceNum(int num)
	{
		WORLD_CHUNK_RENDER_DISTANCE = num;
	}
}
