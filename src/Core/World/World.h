#pragma once
#include "../Util.h"
#include "../Objects/PlayerObj.h"
#include "Renderer/Model/Characters/Player.h"
#include "ChunkGeneration.h"

namespace MyWorld
{
	class World
	{
	private:
		static int zoneChunkNum;
		static int zoneWidth;
		static float selection_distance_blocks;
		static float selection_distance_blocks_square;
		// the position info of selected block
		static glm::vec3 selectedPos;
		// the face of the block selected
		static Block::DIRECTION selectedFace;
		static const glm::vec3 NOT_SELECTED;
		static Wireframe wireframe;
		static PlayerObj player;
		static std::vector<Chunk*> chunks;
		static void Draw();
	public:
		static bool selectionEnabled; // SETTINGS
		static bool collisionEnabled; // SETTINGS
		static bool gravityEnabled; // SETTINGS
		static bool infiniteWorldEnabled; // SETTINGS
		static void Init();
		static void Update();
		static void Destroy();
		static void updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, Block::DIRECTION& direction, float& face, float& offset, float& closestPointLength);
		static void selectBlock();
		static const glm::vec3& getSelectedBlockPos();
		static const Block::DIRECTION& getSelectedFace();
		static PlayerObj& getPlayer();
	};
}
