#pragma once
#include "../Util.h"
#include "Chunk.h"
#include "GameState.h"

namespace MyWorld
{
	class ChunkGeneration
	{
	public:
		static void Start(std::vector<Chunk*>& chunks);
		static void Terminate();
		static std::mutex& ChunkLock();
		static const glm::vec2& BufferZone();
		static const glm::vec2 syncZone(const std::optional<glm::vec2>& zone = std::nullopt);
	private:
		static glm::vec2 bufferZone;
		static std::mutex zone_lock;
		static std::mutex release_lock;
		static std::mutex chunk_lock;
		static std::thread terrain_generation_thread;
		static std::vector<Chunk*> work_in_progress;
		static void Generate(std::vector<Chunk*>& chunks);
		static void ReleaseChunks(std::vector<Chunk*>& to_be_released);
		static bool isOutOfWorld(glm::vec2& coord, glm::vec2& zone);
	};
}