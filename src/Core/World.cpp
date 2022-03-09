#include "World.h"

namespace MyWorld
{
	void World::Generate()
	{
		Data::Init();
	}

	void World::Update()
	{
		for (int i = 0; i < Data::CHUNK_WIDTH_NUM * Data::CHUNK_WIDTH_NUM; i++)
		{
			Data::chunks[i]->Draw();
		}

		for (int i = 0; i < Data::CHUNK_WIDTH_NUM * Data::CHUNK_WIDTH_NUM; i++)
		{
			Data::chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < Data::CHUNK_WIDTH_NUM * Data::CHUNK_WIDTH_NUM; i++)
		{
			Data::chunks[i]->Draw(Chunk::Phase::WATER_P);
		}
	}

	void World::Destroy()
	{
		Data::Destroy();
	}
}
