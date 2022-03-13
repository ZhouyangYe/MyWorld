#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	// TODO: do this in a separate thread
	void Data::Init()
	{
		const int chunkNum = Chunk::getWorldChunkNum();
		for (float y = 0; y < chunkNum; y++)
		{
			for (float x = 0; x < chunkNum; x++)
			{
				chunks.push_back(new Chunk(glm::vec2{x * 16.0f, y * 16.0f }, y * chunkNum + x));
			}
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
