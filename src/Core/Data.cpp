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
		for (int y = 0; y < chunkNum; y++)
		{
			for (int x = 0; x < chunkNum; x++)
			{
				chunks.push_back(new Chunk(glm::vec2{(float)x * 16.0f, (float)y * 16.0f }, y * chunkNum + x));
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
