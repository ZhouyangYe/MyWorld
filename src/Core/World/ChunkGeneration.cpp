#include "ChunkGeneration.h"
#include "ChunkGeneration.h"

namespace MyWorld
{
	std::mutex ChunkGeneration::chunk_lock;
	std::mutex ChunkGeneration::release_lock;
	std::mutex ChunkGeneration::zone_lock;
	std::thread ChunkGeneration::terrain_generation_thread;
	// chunks will not be updated unless player go out of the zone
	glm::vec2 ChunkGeneration::bufferZone;
	std::vector<Chunk*> ChunkGeneration::work_in_progress;

	const glm::vec2& ChunkGeneration::BufferZone()
	{
		return bufferZone;
	}

	std::mutex& ChunkGeneration::ChunkLock()
	{
		return chunk_lock;
	}

	const glm::vec2 ChunkGeneration::syncZone(const std::optional<glm::vec2>& zone)
	{
		std::lock_guard<std::mutex> guard(zone_lock);

		if (zone)
		{
			bufferZone = *zone;
		}

		return bufferZone;
	}

	void ChunkGeneration::ReleaseChunks(std::vector<Chunk*>& to_be_released)
	{
		while (true)
		{
			release_lock.lock();
			int size = to_be_released.size();
			if (GameState::gameover && !size)
			{
				release_lock.unlock();
				break;
			}
			else if (size)
			{
				delete to_be_released[size - 1];
				to_be_released.pop_back();
			}
			release_lock.unlock();

			if (!size)
			{
				// if all chunks are destroyed, take a break;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}

	bool ChunkGeneration::isOutOfWorld(glm::vec2& coord, glm::vec2& zone)
	{
		return
			coord.x < zone.x - TerrainGeneration::ChunkRenderDistanceNum() ||
			coord.x >= zone.x + TerrainGeneration::ChunkRenderDistanceNum() ||
			coord.y < zone.y - TerrainGeneration::ChunkRenderDistanceNum() ||
			coord.y >= zone.y + TerrainGeneration::ChunkRenderDistanceNum();
	}

	// procedurally generate terrains in another thread
	void ChunkGeneration::Generate(std::vector<Chunk*>& chunks)
	{
		std::vector<Chunk*> to_be_released;
		std::unordered_map<glm::vec2, Chunk*> to_be_added;
		std::vector<glm::vec2> to_be_created;
		bool initialized = false;
		glm::vec2 prevZone = syncZone();
		std::thread release_thread = std::thread(ReleaseChunks, std::ref(to_be_released));

		while (!GameState::gameover)
		{
			glm::vec2 currentZone = syncZone();

			// cache chunk data to be created or released later when buffer zone changes
			if (currentZone != prevZone || !initialized)
			{
				to_be_added.clear();
				to_be_created.clear();
				work_in_progress.clear();
				if (!initialized) initialized = true;

				// verify old to_be_released queue, determine which to be release or kept and put them into corresponding data structure
				release_lock.lock();
				const int releaseSize = to_be_released.size();
				if (releaseSize)
				{
					for (int i = 0; i < releaseSize; i++)
					{
						Chunk* chunk = to_be_released[i];
						glm::vec2 coord = chunk->getCoords();
						if (!isOutOfWorld(coord, currentZone))
						{
							to_be_added[coord] = chunk;
							to_be_released.erase(to_be_released.begin() + i);
						}
					}
				}

				// verify old displayed chunks, determine which to be release or kept and put them into corresponding data structure
				const int chunkSize = chunks.size();
				for (int i = 0; i < chunkSize; i++)
				{
					Chunk* chunk = chunks[i];
					glm::vec2 coord = chunk->getCoords();
					if (isOutOfWorld(coord, currentZone))
					{
						to_be_released.push_back(chunk);
					}
					else
					{
						to_be_added[coord] = chunk;
					}
				}

				Util::mergeSort<Chunk*>(to_be_released.data(), to_be_released.size(), [&](Chunk* item1, Chunk* item2) {
					glm::vec2 coord1 = item1->getCoords(), coord2 = item2->getCoords();

					return glm::length2(coord2 - bufferZone) - glm::length2(coord1 - bufferZone);
					});

				// put all remained chunks into work_in_progress, and determine which chunks are yet to be created for later use
				for (int y = -TerrainGeneration::ChunkRenderDistance(); y < TerrainGeneration::ChunkRenderDistance(); y++)
				{
					for (int x = -TerrainGeneration::ChunkRenderDistance(); x < TerrainGeneration::ChunkRenderDistance(); x++)
					{
						glm::vec2 coord = glm::vec2{ x, y } *(float)CHUNK_WIDTH + currentZone;
						if (to_be_added.find(coord) == to_be_added.end())
						{
							to_be_created.push_back(coord);
						}
						else
						{
							work_in_progress.push_back(to_be_added[coord]);
							// to_be_added.erase(coord);
						}
					}
				}
				// swap to use updated chunks
				chunk_lock.lock();
				chunks.swap(work_in_progress);
				chunk_lock.unlock();
				release_lock.unlock();
				// copy current to work_in_progress
				work_in_progress = chunks;

				Util::mergeSort<glm::vec2>(to_be_created.data(), to_be_created.size(), [&](glm::vec2& item1, glm::vec2& item2) {
					return glm::length2(item1 - bufferZone) - glm::length2(item2 - bufferZone);
				});
			}

			int createSize = to_be_created.size();
			if (createSize)
			{
				work_in_progress.push_back(new Chunk(to_be_created[createSize - 1]));
				to_be_created.pop_back();
			}
			else
			{
				// if all chunks are created, take a break
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			// swap to use updated chunks
			chunk_lock.lock();
			chunks.swap(work_in_progress);
			chunk_lock.unlock();
			work_in_progress = chunks;

			prevZone = currentZone;
		}

		release_thread.join();
		int releaseSize = to_be_released.size();
		if (releaseSize)
		{
			for (int i = 0; i < releaseSize; i++)
			{
				delete to_be_released[i];
			}
		}
	}

	void ChunkGeneration::Start(std::vector<Chunk*>& chunks)
	{
		bufferZone = glm::vec2{ 0.0f, 0.0f };

		terrain_generation_thread = std::thread(Generate, std::ref(chunks));
	}

	void ChunkGeneration::Terminate()
	{
		terrain_generation_thread.join();
		for (std::vector<Chunk*>::iterator iter = work_in_progress.begin(); iter != work_in_progress.end(); ++iter)
		{
			delete (*iter);
		}
		std::vector<Chunk*>().swap(work_in_progress);
	}
}