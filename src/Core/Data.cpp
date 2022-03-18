#include "Data.h"

namespace MyWorld
{
	std::vector<Chunk*> Data::chunks;

	Data::Data()
	{}

	// TODO: do this in a separate thread
	void Data::Init(bool& infiniteWorldEnabled)
	{
		Chunk::setShowWorldBorder(!infiniteWorldEnabled);
		const int chunkNum = Chunk::getChunkRenderDistanceNum();
		int index = 0;
		for (int y = -chunkNum; y < chunkNum; y++)
		{
			for (int x = -chunkNum; x < chunkNum; x++)
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
		chunks.clear();
	}
}
