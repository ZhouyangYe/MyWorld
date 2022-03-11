#pragma once
#include "Data.h"

namespace MyWorld
{
	class World
	{
	private:
		static int chunk_num;
	public:
		static void Generate();
		static void Update();
		static void Destroy();
	};
}
