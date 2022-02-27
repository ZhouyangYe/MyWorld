#pragma once
#include "Chunk.h"

namespace MyWorld
{
	class Data
	{
	private:
	public:
		static std::vector<Chunk*> chunks;
		static const int CHUNK_LENGTH;
		Data();
		static void Init();
		static void Destroy();
	};
}
