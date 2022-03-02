#include "Chunk.h"

namespace MyWorld
{
	const int Chunk::CHUNK_DEPTH = 386;
	const int Chunk::CHUNK_WIDTH = 16;
	bool Chunk::showEdge = true;
	FastNoiseLite Chunk::noise;

	// get block's faces' distance to camera
	float Chunk::getLength(Block* block)
	{
		const glm::vec3 coords = block->getCoords();
		glm::vec3 center;
		switch (block->faces)
		{
		case Block::DIRECTION::NORTH:
			center = coords + Block::NorthFaceVec;
			break;
		case Block::DIRECTION::SOUTH:
			center = coords + Block::SouthFaceVec;
			break;
		case Block::DIRECTION::WEST:
			center = coords + Block::WestFaceVec;
			break;
		case Block::DIRECTION::EAST:
			center = coords + Block::EastFaceVec;
			break;
		case Block::DIRECTION::TOP:
			center = coords + Block::TopFaceVec;
			break;
		case Block::DIRECTION::BOTTOM:
			center = coords + Block::BottomFaceVec;
			break;
		default:
			break;
		}

		return glm::length2(center - Camera::getCameraCoords());
	}

	void Chunk::faceCullingAndSeparating()
	{
		// figure out which faces to be drawn
		for (int i = 0; i < blocks.size(); i++)
		{
			if (blocks[i]->type != Block::AIR)
			{
				Block* block = blocks[i];
				Block::TYPE type = block->type;
				glm::vec3 blockCoords = block->getCoords();

				// TODO: frustum culling
				// TODO: optimize it!
				if (blockCoords.z == 0 || blocks[i - 1]->type == Block::AIR || (type != Block::WATER && blocks[i - 1]->type == Block::WATER))
				{
					if (showEdge || blockCoords.z != 0)
						block->faces |= Block::DIRECTION::BOTTOM;
				}
				if (blockCoords.z == CHUNK_DEPTH - 1 || blocks[i + 1]->type == Block::AIR || (type != Block::WATER && blocks[i + 1]->type == Block::WATER))
				{
					if (showEdge || blockCoords.z != CHUNK_DEPTH - 1)
						block->faces |= Block::DIRECTION::TOP;
				}

				if (blockCoords.x == coords.x || blocks[i - X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i - X_OFFSET]->type == Block::WATER))
				{
					if (type == Block::WATER || showEdge || blockCoords.x != coords.x)
						block->faces |= Block::DIRECTION::WEST;
				}
				if (blockCoords.x == coords.x + CHUNK_WIDTH - 1 || blocks[i + X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i + X_OFFSET]->type == Block::WATER))
				{
					if (type == Block::WATER || showEdge || blockCoords.x != coords.x + CHUNK_WIDTH - 1)
						block->faces |= Block::DIRECTION::EAST;
				}

				if (blockCoords.y == coords.y || blocks[i - Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i - Y_OFFSET]->type == Block::WATER))
				{
					if (type == Block::WATER || showEdge || blockCoords.y != coords.y)
						block->faces |= Block::DIRECTION::SOUTH;
				}
				if (blockCoords.y == coords.y + CHUNK_WIDTH - 1 || blocks[i + Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i + Y_OFFSET]->type == Block::WATER))
				{
					if (type == Block::WATER || showEdge || blockCoords.y != coords.y + CHUNK_WIDTH - 1)
						block->faces |= Block::DIRECTION::NORTH;
				}

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
								Water* water = new Water(block->getCoords());
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

	Chunk::Chunk(glm::vec2 coords = glm::vec2{ 0.0f, 0.0f }) : coords(coords)
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
							blocks.push_back(new Grass({ (float)x + coords.x, (float)y + coords.y, (float)z }));
						}
						else
						{
							blocks.push_back(new Dirt({ (float)x + coords.x, (float)y + coords.y, (float)z }));
						}
					}
					else if (z < 193)
					{
						blocks.push_back(new Water({ (float)x + coords.x, (float)y + coords.y, (float)z }));
					}
					else
					{
						blocks.push_back(new Air({ (float)x + coords.x, (float)y + coords.y, (float)z }));
					}
				}
			}
		}

		faceCullingAndSeparating();
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
	}

	void Chunk::Draw()
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
			// draw far blocks first
			mergeSort<Block*>(sortedBlocks, size, [](Block* item1, Block* item2) {
				return getLength(item1) - getLength(item2);
			});
			for (int i = size - 1; i >= 0; i--)
			{
				sortedBlocks[i]->Draw(sortedBlocks[i]->faces);
			}
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
