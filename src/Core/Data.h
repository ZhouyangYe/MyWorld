#pragma once
#include "Chunk.h"

namespace MyWorld
{
	class Data
	{
	private:
		struct DataParam
		{
			bool infiniteWorldEnabled;
			int renderDistance;
		};
	public:
		static std::vector<Chunk*> chunks;
		Data();
		static void Init(DataParam param);
		static void Destroy();
	};
}
