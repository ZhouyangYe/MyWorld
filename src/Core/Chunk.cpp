#include "Chunk.h"
#include <bitset>

namespace MyWorld
{
	const int Chunk::CHUNK_DEPTH = 256;
	const int Chunk::CHUNK_WIDTH = 16;
	FastNoiseLite Chunk::noise;

	void Chunk::Init()
	{
		// register blocks
		Block::Register();
		Grass::Register();
		Dirt::Register();
		Water::Register();

		noise.SetSeed(6666);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.015);
		noise.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);
		noise.SetFractalOctaves(8);
		noise.SetFractalLacunarity(2.1);
		noise.SetFractalGain(0.5);
		noise.SetFractalWeightedStrength(0.1);
		noise.SetFractalPingPongStrength(1.5);
	}

	void Chunk::Terminate()
	{
		// destroy blocks
		Block::Destroy();
		Grass::Destroy();
		Dirt::Destroy();
		Water::Destroy();
	}

	Chunk::Chunk(glm::vec3 coords)
	{
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				const float data = (noise.GetNoise((float)x, (float)y) + 1) / 2;
				const int surface = (int)(CHUNK_DEPTH * data);
				const int edge = surface - 1;

				for (int z = 0; z < CHUNK_DEPTH; z++)
				{
					// TODO: delete this!
					if (x == 0 && y == 15 && z == 125)
					{
						Water* waterBlock = new Water({ (float)x, (float)y, (float)z });
						blocks.push_back(waterBlock);
						transparent_blocks.push_back(waterBlock);
						continue;
					}

					if (z < surface)
					{
						if (z == edge && z >= 119)
						{
							blocks.push_back(new Grass({ (float)x, (float)y, (float)z }));
						}
						else
						{
							blocks.push_back(new Dirt({ (float)x, (float)y, (float)z }));
						}
					}
					else if (z < 120)
					{
						blocks.push_back(new Water({ (float)x, (float)y, (float)z }));
					}
					else
					{
						blocks.push_back(new Air({ (float)x, (float)y, (float)z }));
					}
				}
			}
		}

		for (int i = 0; i < blocks.size(); i++)
		{
			if (blocks[i]->type != Block::AIR)
			{
				Block* block = blocks[i];
				Block::TYPE type = block->type;
				glm::vec3 coords = block->getCoords();

				// TODO: frustum culling
				// TODO: optimize it!
				if (coords.z == 0 || blocks[i - 1]->type == Block::AIR || (type != Block::WATER && blocks[i - 1]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::BOTTOM;
				}
				if (coords.z == CHUNK_DEPTH - 1 || blocks[i + 1]->type == Block::AIR || (type != Block::WATER && blocks[i + 1]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::TOP;
				}

				if (coords.x == 0 || blocks[i - X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i - X_OFFSET]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::WEST;
				}
				if (coords.x == CHUNK_WIDTH - 1 || blocks[i + X_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i + X_OFFSET]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::EAST;
				}

				if (coords.y == 0 || blocks[i - Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i - Y_OFFSET]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::SOUTH;
				}
				if (coords.y == CHUNK_WIDTH - 1 || blocks[i + Y_OFFSET]->type == Block::AIR || (type != Block::WATER && blocks[i + Y_OFFSET]->type == Block::WATER))
				{
					block->faces |= Block::DIRECTION::NORTH;
				}

				// draw non-transparent blocks first
				if (type == Block::WATER)
				{
					if (block->faces != 0) transparent_blocks.push_back(block);
					continue;
				}

				if (block->faces != 0) blocks[i]->Draw(block->faces);
			}
		}
	}

	Chunk::~Chunk()
	{
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter) {
			delete *iter;
		}
	}

	void Chunk::Draw()
	{
		// draw opaque blocks first
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->Draw(blocks[i]->faces);
		}

		// draw transparent blocks
		int size = transparent_blocks.size();
		Block** sortedBlocks = transparent_blocks.data();
		mergeSort<Block*>(sortedBlocks, size, [](Block* item1, Block* item2) {
			return glm::length(item1->getCoords() - Camera::getCameraCoords()) - glm::length(item2->getCoords() - Camera::getCameraCoords());
		});
		for (int i = 0; i < size; i++)
		{
			sortedBlocks[i]->Draw(sortedBlocks[i]->faces);
		}
	}
}
