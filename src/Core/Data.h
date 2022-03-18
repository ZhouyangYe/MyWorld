#pragma once
#include "Chunk.h"

namespace MyWorld
{
	class Data
	{
	private:
	public:
		static std::vector<Chunk*> chunks;
		Data();
		static void Init(bool& infiniteWorldEnabled, int& renderDistance);
		static void Destroy();
	};
}
