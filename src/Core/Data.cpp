#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk> Data::chunks;

	Data::Data()
	{}

	// TODO: do this in a separate thread
	void Data::Init(bool& infiniteWorldEnabled, int& renderDistance)
	{
		Chunk::setChunkRenderDistanceNum(renderDistance);
		Chunk::setShowWorldBorder(!infiniteWorldEnabled);

		int index = 0;
		chunks.reserve(renderDistance * renderDistance * 4);
		for (int y = -renderDistance; y < renderDistance; y++)
		{
			for (int x = -renderDistance; x < renderDistance; x++)
			{
				chunks.emplace_back(glm::vec2{ (float)x * 16.0f, (float)y * 16.0f }, index);
				index++;
			}
		}
	}

	void Data::Destroy()
	{
		std::vector<Chunk>().swap(chunks);
	}
}
