#pragma once
#include "Util.h"
#include "Objects/PlayerObj.h"
#include "Renderer/Model/Characters/Player.h"

namespace MyWorld
{
	class World
	{
	private:
		static std::mutex zone_lock;
		static glm::vec2 bufferZone;
		static int zoneChunkNum;
		static int zoneWidth;
		static int renderDistance; // SETTINGS
		static int renderDistanceNum;
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
		static std::mutex release_lock;
		static std::mutex chunk_lock;
		static std::vector<Chunk*> current;
		static std::vector<Chunk*> work_in_progress;
		static std::thread terrain_generation_thread;
		static const glm::vec2 syncZone(const std::optional<glm::vec2>& zone);
		static bool isOutOfWorld(glm::vec2& coord, glm::vec2& zone);
		static void Draw();
		static void Generate();
		static void ReleaseChunks(std::vector<Chunk*>& to_be_released);
	public:
		static std::atomic_bool gameover;
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
		static const int& getRenderDistance();
		static PlayerObj& getPlayer();
	};
}
