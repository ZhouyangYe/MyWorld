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
		static void updateClosestPoint(bool& blockFound, float& closestPointLength, const Block::DIRECTION& direction, bool& outOfRange, int& face, const int& offset, const glm::vec3& forwardVec, const glm::vec3& eyeLocation);
		static void selectBlock();
	};
}
