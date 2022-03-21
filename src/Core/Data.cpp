#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	// TODO: do this in a separate thread
	void Data::Init(DataParam param)
	{
		Chunk::setChunkRenderDistanceNum(param.renderDistance);
		Chunk::setShowWorldBorder(!param.infiniteWorldEnabled);

		int index = 0;
		chunks.reserve(param.renderDistance * param.renderDistance * 4);
		for (int y = -param.renderDistance; y < param.renderDistance; y++)
		{
			for (int x = -param.renderDistance; x < param.renderDistance; x++)
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
