#pragma once
#include <PerlinNoise.hpp>

#include "Renderer/Model/Blocks/Block.h"
#include "Renderer/Model/Blocks/Air.h"
#include "Renderer/Model/Blocks/Grass.h"
#include "Renderer/Model/Blocks/Dirt.h"

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

namespace MyWorld
{
	class Chunk
	{
	private:
		static const siv::PerlinNoise perlin;
		std::vector<Block*> blocks;
	public:
		Chunk(glm::vec3 coords);
		~Chunk();
		void Draw();
	};
}
