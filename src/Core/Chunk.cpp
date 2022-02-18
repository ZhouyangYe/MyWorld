#include "Chunk.h"

namespace MyWorld
{
	const siv::PerlinNoise Chunk::perlin{ 123456u };

	Chunk::Chunk(glm::vec3 coords)
	{
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
				int surface = (int)(CHUNK_DEPTH * noise);
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
		uint8_t faces = Block::DIRECTION::NORTH | Block::DIRECTION::SOUTH | Block::DIRECTION::WEST | Block::DIRECTION::EAST | Block::DIRECTION::TOP | Block::DIRECTION::BOTTOM;
		for (std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter) {
			if ((*iter)->type != Block::AIR)
				(*iter)->Draw(faces);
		}
	}
}
