#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	void Data::Init()
	{
		for (int y = 0; y < Chunk::WORLD_CHUNK_NUM; y++)
		{
			for (int x = 0; x < Chunk::WORLD_CHUNK_NUM; x++)
			{
				chunks.push_back(new Chunk(glm::vec2{(float)x * 16.0f, (float)y * 16.0f }, y * Chunk::WORLD_CHUNK_NUM + x));
			}
		}

		for (std::vector<Chunk*>::iterator iter = chunks.begin(); iter != chunks.end(); ++iter)
		{
			(*iter)->Build(&chunks);
		}
	}

	void Data::Destroy()
	{
		for (std::vector<Chunk*>::iterator iter = chunks.begin(); iter != chunks.end(); ++iter)
		{
			delete (*iter);
		}
		chunks.clear();
	}
}
