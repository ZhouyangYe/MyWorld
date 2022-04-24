#pragma once
#include "Objects/PlayerObj.h"
#include "Renderer/Model/Characters/Player.h"

namespace MyWorld
{
	class World
	{
	private:
		static glm::vec2 region[4];
		static int renderDistance; // SETTINGS
		static float selection_distance_blocks;
		static float selection_distance_blocks_square;
		static int chunk_num;
		// the position info of selected block
		static glm::vec3 selectedPos;
		// the face of the block selected
		static Block::DIRECTION selectedFace;
		static const glm::vec3 NOT_SELECTED;
		static Wireframe wireframe;
		static PlayerObj player;
		static std::vector<Chunk*> chunks;
	public:
		static bool selectionEnabled; // SETTINGS
		static bool collisionEnabled; // SETTINGS
		static bool gravityEnabled; // SETTINGS
		static bool infiniteWorldEnabled; // SETTINGS
		static void Generate();
		static void Update();
		static void Destroy();
		static void updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, Block::DIRECTION& direction, float& face, float& offset, float& closestPointLength);
		static void selectBlock();
		static const glm::vec3& getSelectedBlockPos();
		static const Block::DIRECTION& getSelectedFace();
		static const int& getRenderDistance();
		static PlayerObj& getPlayer();
	};
}
