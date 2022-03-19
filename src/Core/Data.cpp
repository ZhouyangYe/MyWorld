#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	// TODO: do this in a separate thread
	void Data::Init(bool& infiniteWorldEnabled, int& renderDistance)
	{
		Chunk::setChunkRenderDistanceNum(renderDistance);
		Chunk::setShowWorldBorder(!infiniteWorldEnabled);
		int index = 0;
		for (int y = -renderDistance; y < renderDistance; y++)
		{
			for (int x = -renderDistance; x < renderDistance; x++)
			{
				chunks.push_back(new Chunk(glm::vec2{ (float)x * 16.0f, (float)y * 16.0f }, index));
				index++;
			}
		}
	}

	void Data::Destroy()
	{
		for (std::vector<Chunk*>::iterator iter = chunks.begin(); iter != chunks.end(); ++iter)
		{
			delete (*iter);
		}
		std::vector<Chunk*>().swap(chunks);
	}
}
