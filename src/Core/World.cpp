#include "World.h"

namespace MyWorld
{
	int World::chunk_num;

	void World::Generate()
	{
		Data::Init();

		const int num = Chunk::getChunkRenderDistanceNum();
		World::chunk_num = num * num * 4;
	}

	void World::Update()
	{
		// TODO: optimize this
		// if greedy meshing is not used, draw blocks one by one
		if (!Texture::isArrayBufferSupported())
		{
			for (int i = 0; i < chunk_num; i++)
			{
				Data::chunks[i]->Draw();
			}
		}
		// draw chunks of greedy meshed and batched vertices
		else
		{
			for (int i = 0; i < chunk_num; i++)
			{
				Data::chunks[i]->Draw(Chunk::Phase::OPAQUE_P);
			}

			for (int i = 0; i < chunk_num; i++)
			{
				Data::chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
			}

			for (int i = 0; i < chunk_num; i++)
			{
				Data::chunks[i]->Draw(Chunk::Phase::WATER_P);
			}
		}

		Chunk::DrawTransparent();
	}

	void World::Destroy()
	{
		Data::Destroy();
	}
}
