#include "Chunk.h"
#include <bitset>

namespace MyWorld
{
	const int Chunk::CHUNK_DEPTH = 386;
	const int Chunk::CHUNK_WIDTH = 16;
	bool Chunk::showEdge = true;
	FastNoiseLite Chunk::noise;

	// get block's faces' distance to camera
	float Chunk::getLength(Block* block)
	{
		const glm::vec3 blockCoords = block->getCalculatedCoords();
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

	const bool Chunk::has(Block::DIRECTION face, const int& idx)
	{
		Block* block = blocks[idx];
		Block::TYPE type = block->type;
		glm::vec3 blockCoords = block->getCoords();

		switch (face)
		{
		case MyWorld::Block::NORTH:
			return 
				(blockCoords.y == CHUNK_WIDTH - 1 || blocks[idx + Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx + Y_OFFSET]->type == Block::WATER))
				&& (type == Block::WATER || showEdge || blockCoords.y != 0 + CHUNK_WIDTH - 1);
		case MyWorld::Block::SOUTH:
			return 
				(blockCoords.y == 0 || blocks[idx - Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx - Y_OFFSET]->type == Block::WATER))
				&& (type == Block::WATER || showEdge || blockCoords.y != 0);
		case MyWorld::Block::WEST:
			return 
				(blockCoords.x == 0 || blocks[idx - X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx - X_OFFSET]->type == Block::WATER))
				&& (type == Block::WATER || showEdge || blockCoords.x != 0);
		case MyWorld::Block::EAST:
			return 
				(blockCoords.x == CHUNK_WIDTH - 1 || blocks[idx + X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx + X_OFFSET]->type == Block::WATER))
				&& (type == Block::WATER || showEdge || blockCoords.x != CHUNK_WIDTH - 1);
		case MyWorld::Block::TOP:
			return 
				(blockCoords.z == CHUNK_DEPTH - 1 || blocks[idx + Z_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx + Z_OFFSET]->type == Block::WATER))
				&& (showEdge || blockCoords.z != CHUNK_DEPTH - 1);
		case MyWorld::Block::BOTTOM:
			return 
				(blockCoords.z == 0 || blocks[idx - Z_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[idx - Z_OFFSET]->type == Block::WATER))
				&& (showEdge || blockCoords.z != 0);
		default:
			return false;
		}
	}

	void Chunk::batchFacesForType1(const Block::PosTextureArrayVertex* vertices)
	{
		for (int i = 0; i < Block::faceVerticesNum; i++)
		{
			batched_model_vertices_type1.push_back(vertices[i]);
		}

		for (int i = 0; i < Block::faceVerticesNum + 2; i++)
		{
			batched_model_index_type1.push_back(Block::faceIndex[i] + batching_index_type1 * Block::faceVerticesNum);
		}

		delete[] vertices;

		batching_index_type1++;
	}

	void Chunk::batchFacesForType2(const Block::PosColorTextureArrayVertex* vertices)
	{
		for (int i = 0; i < Block::faceVerticesNum; i++)
		{
			batched_model_vertices_type2.push_back(vertices[i]);
		}

		for (int i = 0; i < Block::faceVerticesNum + 2; i++)
		{
			batched_model_index_type2.push_back(Block::faceIndex[i] + batching_index_type2 * Block::faceVerticesNum);
		}

		delete[] vertices;

		batching_index_type2++;
	}

	void Chunk::createBatchingOfFaces(Block* startBlock, Block* endBlock, Block::DIRECTION direction)
	{
		if (direction == Block::DIRECTION::EAST)
		{
			glm::vec3 start = startBlock->getCoords();
			glm::vec3 end = endBlock->getCoords();
			std::cout << start.x << "-" << start.y << "-" << start.z << "  " << end.x << "-" << end.y << "-" << end.z << "\n";
		}

		switch (startBlock->type)
		{
		case Block::DIRT:
		{
			const Block::PosTextureArrayVertex* vertices_dirt = Dirt::getFaceVertices(startBlock, endBlock, direction);
			batchFacesForType1(vertices_dirt);
			break;
		}
		case Block::GRASS:
		{
			const Block::PosTextureArrayVertex* vertices_grass = Grass::getFaceVertices(startBlock, endBlock, direction);
			batchFacesForType1(vertices_grass);
			break;
		}
		case Block::WATER:
		{
			const Block::PosColorTextureArrayVertex* vertices_water = Water::getFaceVertices(startBlock, endBlock, direction);
			batchFacesForType2(vertices_water);
			break;
		}
		default:
			break;
		}
	}

	// greedy meshing for faces
	void Chunk::greedyMergeFaces(Block::DIRECTION face, const int& idx)
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

	// figure out which faces to be drawn, merge blocks, and put opaque and transparent blocks into different arrays
	void Chunk::faceCullingAndSeparating()
	{
		// TODO: frustum culling
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
									Water* water = new Water(block->getCoords(), coords);
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

	void Chunk::toggleFaceCulling()
	{
		showEdge = !showEdge;
	}

	void Chunk::Init()
	{
		// register blocks
		Block::Register();
		Grass::Register();
		Dirt::Register();
		Water::Register();

		noise.SetSeed(666666);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.003f);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFractalOctaves(4);
		noise.SetFractalLacunarity(1.5f);
		noise.SetFractalGain(1.4f);
		noise.SetFractalWeightedStrength(0.4f);
		// noise.SetFractalPingPongStrength(1.5f);
	}

	void Chunk::Destroy()
	{
		// destroy blocks
		Block::Destroy();
		Grass::Destroy();
		Dirt::Destroy();
		Water::Destroy();
	}

	Chunk::Chunk()
	{}

	Chunk::Chunk(glm::vec2 coords = glm::vec2{ 0.0f, 0.0f }) : 
		coords(glm::vec3(coords, 0.0f)), 
		vbh_type1(BGFX_INVALID_HANDLE),
		ibh_type1(BGFX_INVALID_HANDLE),
		vbh_type2(BGFX_INVALID_HANDLE),
		ibh_type2(BGFX_INVALID_HANDLE),
		program_type1(BGFX_INVALID_HANDLE),
		program_type2(BGFX_INVALID_HANDLE),
		batching_index_type1(0),
		batching_index_type2(0)
	{
		// create all blocks in the chunk
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				const float data = (noise.GetNoise((float)x + coords.x, (float)y + coords.y) + 1) / 2;
				const int surface = (int)(CHUNK_DEPTH * data);
				const int edge = surface - 1;

				for (int z = 0; z < CHUNK_DEPTH; z++)
				{
					if (z < surface)
					{
						if (z == edge && z >= 192)
						{
							blocks.push_back(new Grass({ (float)x, (float)y, (float)z }, coords));
						}
						else
						{
							blocks.push_back(new Dirt({ (float)x, (float)y, (float)z }, coords));
						}
					}
					else if (z < 193)
					{
						blocks.push_back(new Water({ (float)x, (float)y, (float)z }, coords));
					}
					else
					{
						blocks.push_back(new Air({ (float)x, (float)y, (float)z }, coords));
					}
				}
			}
		}

		faceCullingAndSeparating();

		if (Texture::isArrayBufferSupported())
		{
			program_type1 = Renderer::texture_array_program;
			vbh_type1 = bgfx::createVertexBuffer(bgfx::makeRef(batched_model_vertices_type1.data(), batched_model_vertices_type1.size() * sizeof(Block::PosTextureArrayVertex)), Renderer::getTextureArrayLayout());
			ibh_type1 = bgfx::createIndexBuffer(bgfx::makeRef(batched_model_index_type1.data(), batched_model_index_type1.size() * sizeof(uint16_t)));
			program_type2 = Renderer::texture_array_color_program;
			vbh_type2 = bgfx::createVertexBuffer(bgfx::makeRef(batched_model_vertices_type2.data(), batched_model_vertices_type2.size() * sizeof(Block::PosColorTextureArrayVertex)), Renderer::getColorTextureArrayLayout());
			ibh_type2 = bgfx::createIndexBuffer(bgfx::makeRef(batched_model_index_type2.data(), batched_model_index_type2.size() * sizeof(uint16_t)));
		}
	}

	Chunk::~Chunk()
	{
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		{
			delete *iter;
		}
		for (std::vector<Block*>::iterator iter = transparent_blocks.begin(); iter != transparent_blocks.end(); ++iter)
		{
			delete *iter;
		}

		if (bgfx::isValid(vbh_type1)) bgfx::destroy(vbh_type1);
		if (bgfx::isValid(ibh_type1)) bgfx::destroy(ibh_type1);
		if (bgfx::isValid(program_type1)) bgfx::destroy(program_type1);

		if (bgfx::isValid(vbh_type2)) bgfx::destroy(vbh_type2);
		if (bgfx::isValid(ibh_type2)) bgfx::destroy(ibh_type2);
		if (bgfx::isValid(program_type2)) bgfx::destroy(program_type2);
	}

	void Chunk::Draw()
	{
		if (!Texture::isArrayBufferSupported())
		{
			// draw opaque blocks first
			for (int i = 0; i < opaque_blocks.size(); i++)
			{
				opaque_blocks[i]->Draw(opaque_blocks[i]->faces);
			}

			// draw transparent blocks
			int size = transparent_blocks.size();
			if (size)
			{
				Block** sortedBlocks = transparent_blocks.data();
				// draw far faces first
				mergeSort<Block*>(sortedBlocks, size, [](Block* item1, Block* item2) {
					return getLength(item1) - getLength(item2);
					});
				for (int i = size - 1; i >= 0; i--)
				{
					sortedBlocks[i]->Draw(sortedBlocks[i]->faces);
				}
			}
		}
		else
		{
			Block::DrawTerrain(vbh_type1, ibh_type1, program_type1, Block::default_state, coords);
		}
	}

	void Chunk::toggleEdge()
	{
		transparent_blocks.clear();
		opaque_blocks.clear();
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		{
			(*iter)->faces = 0;
		}

		faceCullingAndSeparating();
	}
}
