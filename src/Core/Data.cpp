#include "Data.h"

namespace MyWorld
{
	const int Data::CHUNK_LENGTH = 2;
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	void Data::Init()
	{
		Chunk::Init();

		for (int i = 0; i < CHUNK_LENGTH; i++)
		{
			chunks.push_back(new Chunk(glm::vec2{ 0.0f + (float)i * 16.0f, 0.0f + (float)i * 16.0f }));
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
