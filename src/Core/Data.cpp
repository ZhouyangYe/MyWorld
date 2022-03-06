#include "Data.h"

namespace MyWorld
{
	const int Data::CHUNK_WIDTH_NUM = 2;
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	void Data::Init()
	{
		Chunk::Init();

		for (int i = 0; i < CHUNK_WIDTH_NUM; i++)
		{
			for (int j = 0; j < CHUNK_WIDTH_NUM; j++)
			{
				if (i == 0 && j == 0) chunks.push_back(new Chunk(glm::vec2{(float)i * 16.0f, (float)j * 16.0f }));
			}
		}
	}

	void Data::Destroy()
	{
		for (std::vector<Chunk*>::iterator iter = chunks.begin(); iter != chunks.end(); ++iter)
		{
			delete* iter;
		}
		Chunk::Destroy();
	}
}
