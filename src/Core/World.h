#pragma once
#include "Data.h"

namespace MyWorld
{
	class World
	{
	private:
		static float distance_blocks;
		static float distance_blocks_square;
		static int chunk_num;
		// the position of selected block
		static glm::vec3 selectedPos;
		static const glm::vec3 NOT_SELECTED;
		static Wireframe* wireframe;
	public:
		static void Generate();
		static void Update();
		static void Destroy();
		static void updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, float& face, float& offset, float& closestPointLength);
		static void selectBlock();
	};
}
