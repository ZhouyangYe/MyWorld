#include "World.h"

namespace MyWorld
{
	int World::chunk_num;
	Wireframe* World::wireframe = nullptr;

	void World::Generate()
	{
		Data::Init();
		wireframe = new Wireframe(glm::vec3{ 0.0f, 0.0f, 192.0f }, glm::vec3{0.0f, 0.0f, 0.0f});

		const int num = Chunk::getChunkRenderDistanceNum();
		World::chunk_num = num * num * 4;
	}

	void World::Update()
	{
		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::OPAQUE_P);
		}

		wireframe->Draw();

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_P);
		}

		Chunk::DrawTransparent();
	}

	void World::Destroy()
	{

		Data::Destroy();
	}
}
