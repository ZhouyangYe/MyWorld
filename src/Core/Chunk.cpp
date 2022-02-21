#include "Chunk.h"

namespace MyWorld
{
	const int Chunk::CHUNK_DEPTH = 256;
	const int Chunk::CHUNK_WIDTH = 16;
	FastNoiseLite Chunk::noise;

	void Chunk::Init()
	{
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	}

	Chunk::Chunk(glm::vec3 coords)
	{
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				const float data = noise.GetNoise((float)x, (float)y);
				int surface = (int)(CHUNK_DEPTH * data);
				int edge = surface - 1;

				for (int z = 0; z < CHUNK_DEPTH; z++)
				{
					if (z < surface)
					{
						if (z == edge)
						{
							blocks.push_back(new Grass({ (float)x, (float)y, (float)z }));
						}
						else
						{
							blocks.push_back(new Dirt({ (float)x, (float)y, (float)z }));
						}
					}
					else
					{
						blocks.push_back(new Air({ (float)x, (float)y, (float)z }));
					}
				}
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
		for (int i = 0; i < blocks.size(); i++)
		{
			if (blocks[i]->type != Block::AIR)
			{
				uint8_t faces = 0;
				glm::vec3 coords = blocks[i]->getCoords();

				// TODO: frustum culling
				// TODO: optimize it!
				if (coords.z == 0 || blocks[i - 1]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::BOTTOM;
				}
				if (coords.z == CHUNK_DEPTH - 1 || blocks[i + 1]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::TOP;
				}

				if (coords.x == 0 || blocks[i - X_OFFSET]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::WEST;
				}
				if (coords.x == CHUNK_WIDTH - 1 || blocks[i + X_OFFSET]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::EAST;
				}

				if (coords.y == 0 || blocks[i - Y_OFFSET]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::SOUTH;
				}
				if (coords.y == CHUNK_WIDTH - 1 || blocks[i + Y_OFFSET]->type == Block::AIR)
				{
					faces |= Block::DIRECTION::NORTH;
				}

				if (faces != 0) blocks[i]->Draw(faces);
			}
		}
	}
}
