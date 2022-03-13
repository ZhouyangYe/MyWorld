#include "Chunk.h"
#include <bitset>

namespace MyWorld
{
	// WORLD_CHUNK_NUM * WORLD_CHUNK_NUM chunks to be rendered
	int Chunk::WORLD_CHUNK_NUM = 8;
	// CHUNK_DEPTH blocks high and CHUNK_WIDTH blocks wide
	const int Chunk::CHUNK_DEPTH = 386;
	const int Chunk::CHUNK_WIDTH = 16;

	const int Chunk::X_OFFSET = CHUNK_DEPTH;
	const int Chunk::Y_OFFSET = CHUNK_WIDTH * CHUNK_DEPTH;
	const int Chunk::Z_OFFSET = 1;

	bool Chunk::showWorldBorder = true;
	FastNoiseLite Chunk::noise;
	std::vector<Block*> Chunk::transparent_blocks;

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

		bool noAdjacent = false;
		float offset = 0.0f;
		Block::TYPE adjacentType = Block::INVALID;

		switch (face)
		{
		case MyWorld::Block::NORTH:
			noAdjacent = index >= WORLD_CHUNK_NUM * (WORLD_CHUNK_NUM - 1);
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
			return (isWorldBorder || adjacentIsAir() || adjacentIsWater()) && (type == Block::WATER || showWorldBorder || !isWorldBorder);
		case MyWorld::Block::SOUTH:
			noAdjacent = index < WORLD_CHUNK_NUM;
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
			return (isWorldBorder || adjacentIsAir() || adjacentIsWater()) && (type == Block::WATER || showWorldBorder || !isWorldBorder);
		case MyWorld::Block::WEST:
			noAdjacent = index % WORLD_CHUNK_NUM == 0;
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
			return (isWorldBorder || adjacentIsAir() || adjacentIsWater()) && (type == Block::WATER || showWorldBorder || !isWorldBorder);
		case MyWorld::Block::EAST:
			noAdjacent = index % WORLD_CHUNK_NUM == WORLD_CHUNK_NUM - 1;
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
			return (isWorldBorder || adjacentIsAir() || adjacentIsWater()) && (type == Block::WATER || showWorldBorder || !isWorldBorder);
		case MyWorld::Block::TOP:
			isWorldBorder = blockCoords.z == CHUNK_DEPTH - 1;
			adjacentIsAir = [&] { return blocks[idx + Z_OFFSET]->type == Block::AIR; };
			adjacentIsWater = [&] { return type != Block::WATER && blocks[idx + Z_OFFSET]->type == Block::WATER; };
			return isWorldBorder || adjacentIsAir() || adjacentIsWater();
		case MyWorld::Block::BOTTOM:
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
			batchFaces(vertices, batched_model_vertices_type1, batched_model_index_type1, batching_index_type1);
			break;
		}
		case Block::GRASS:
		{
			const Renderer::PosTextureArrayVertex* vertices = Grass::getFaceVertices(startBlock, endBlock, direction);
			batchFaces(vertices, batched_model_vertices_type1, batched_model_index_type1, batching_index_type1);
			break;
		}
		case Block::WATER:
		{
			const Renderer::PosTextureArrayVertex* vertices = Water::getFaceVertices(startBlock, endBlock, direction);
			batchFaces(vertices, batched_model_vertices_type2, batched_model_index_type2, batching_index_type2);
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
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::BOTTOM) == 0) greedyMergeFaces(Block::DIRECTION::BOTTOM, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::BOTTOM;
					}
				}
				if (has(Block::DIRECTION::TOP, i))
				{
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::TOP) == 0) greedyMergeFaces(Block::DIRECTION::TOP, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::TOP;
					}
				}

				if (has(Block::DIRECTION::WEST, i))
				{
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::WEST) == 0) greedyMergeFaces(Block::DIRECTION::WEST, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::WEST;
					}
				}
				if (has(Block::DIRECTION::EAST, i))
				{
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::EAST) == 0) greedyMergeFaces(Block::DIRECTION::EAST, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::EAST;
					}
				}

				if (has(Block::DIRECTION::SOUTH, i))
				{
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::SOUTH) == 0) greedyMergeFaces(Block::DIRECTION::SOUTH, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::SOUTH;
					}
				}
				if (has(Block::DIRECTION::NORTH, i))
				{
					if (Texture::isArrayBufferSupported())
					{
						if ((block->faces & Block::DIRECTION::NORTH) == 0) greedyMergeFaces(Block::DIRECTION::NORTH, i);
					}
					else
					{
						block->faces |= Block::DIRECTION::NORTH;
					}
				}

				// if array buffer is not supported, we don't use greedy meshing
				if (!Texture::isArrayBufferSupported())
				{
					// put transparent blocks into a separate array
					if (type == Block::WATER)
					{
						if (block->faces != 0)
						{
							for (int i = 0; i < 6; i++)
							{
								uint8_t face = 1 << i;
								if (block->faces & face)
								{
									Water* water = new Water(block->getCoords(), coords, index);
									water->faces |= face;
									transparent_blocks.push_back(water);
								}
							}
						}
					}
					// put opaque blocks into a separate array
					else if (block->faces != 0)
					{
						opaque_blocks.push_back(block);
					}
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
		// create all blocks in the chunk
		for (float y = 0; y < CHUNK_WIDTH; y++)
		{
			for (float x = 0; x < CHUNK_WIDTH; x++)
			{
				for (float z = 0; z < CHUNK_DEPTH; z++)
				{
					float worldX = x + coords.x, wolrdY = y + coords.y;
					const Block::TYPE type = getType(worldX, wolrdY, z);

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

		Build();
	}

	void Chunk::Build()
	{
		faceCullingAndSeparating();

		if (Texture::isArrayBufferSupported())
		{
			// opaque
			if (batched_model_vertices_type1.size())
			{
				vbh_type1 = bgfx::createVertexBuffer(bgfx::makeRef(batched_model_vertices_type1.data(), batched_model_vertices_type1.size() * sizeof(Renderer::PosTextureArrayVertex)), Renderer::PosTextureArrayVertex::layout);
				ibh_type1 = bgfx::createIndexBuffer(bgfx::makeRef(batched_model_index_type1.data(), batched_model_index_type1.size() * sizeof(uint16_t)));
			}

			// water
			if (batched_model_vertices_type2.size())
			{
				vbh_type2 = bgfx::createVertexBuffer(bgfx::makeRef(batched_model_vertices_type2.data(), batched_model_vertices_type2.size() * sizeof(Renderer::PosTextureArrayVertex)), Renderer::PosTextureArrayVertex::layout);
				ibh_type2 = bgfx::createIndexBuffer(bgfx::makeRef(batched_model_index_type2.data(), batched_model_index_type2.size() * sizeof(uint16_t)));
			}

			// remove blocks when chunk builds are done
			for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
			{
				delete (*iter);
			}
			blocks.clear();
		}
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

	// draw blocks one by one
	void Chunk::Draw()
	{
		// draw opaque blocks
		for (int i = 0; i < opaque_blocks.size(); i++)
		{
			opaque_blocks[i]->Draw(opaque_blocks[i]->faces);
		}
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

	const int& Chunk::getWorldChunkNum()
	{
		return WORLD_CHUNK_NUM;
	}

	void Chunk::setWorldChunkNum(int num)
	{
		WORLD_CHUNK_NUM = num;
	}
}
