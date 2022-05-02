#include "World.h"

namespace MyWorld
{
	std::atomic_bool World::gameover = false;
	bool World::selectionEnabled = true;
	bool World::collisionEnabled = true;
	bool World::gravityEnabled = true;
	bool World::infiniteWorldEnabled = true;
	std::vector<Chunk*> World::current;
	std::vector<Chunk*> World::work_in_progress;
	std::mutex World::chunk_lock;
	std::mutex World::release_lock;
	int World::renderDistance = 3;
	int World::renderDistanceNum = renderDistance * CHUNK_WIDTH;
	int World::chunk_num = renderDistance * renderDistance * 4;
	// chunks will not be updated unless player go out of the zone
	glm::vec2 World::bufferZone;
	int World::zoneChunkNum = 1;
	int World::zoneWidth = zoneChunkNum * CHUNK_WIDTH;
	std::mutex World::zone_lock;
	float World::selection_distance_blocks = 5.0f;
	float World::selection_distance_blocks_square = selection_distance_blocks * selection_distance_blocks;
	Wireframe World::wireframe;
	PlayerObj World::player;
	const glm::vec3 World::NOT_SELECTED = { 0.0f, 0.0f, -1.0f };
	glm::vec3 World::selectedPos = NOT_SELECTED;
	Block::DIRECTION World::selectedFace = Block::DIRECTION::INVALID;
	std::thread World::terrain_generation_thread;

	void World::Init()
	{
		Chunk::setChunkRenderDistanceNum(renderDistanceNum);
		Chunk::setShowWorldBorder(!infiniteWorldEnabled);

		if (infiniteWorldEnabled)
		{
			current.reserve(4);
			for (int y = -zoneChunkNum; y < zoneChunkNum; y++)
			{
				for (int x = -zoneChunkNum; x < zoneChunkNum; x++)
				{
					current.push_back(new Chunk(glm::vec2{ (float)(x * CHUNK_WIDTH), (float)(y * CHUNK_WIDTH) }));
				}
			}

			bufferZone = glm::vec2{ 0.0f, 0.0f };

			// generate terrain in terrain generation thread
			terrain_generation_thread = std::thread(Generate);
		}
		else
		{
			current.reserve(chunk_num);
			for (int y = -renderDistance; y < renderDistance; y++)
			{
				for (int x = -renderDistance; x < renderDistance; x++)
				{
					current.push_back(new Chunk(glm::vec2{ (float)(x * CHUNK_WIDTH), (float)(y * CHUNK_WIDTH) }));
				}
			}
		}

		player.setPos(Chunk::getSpawnLocation() - PlayerObj::posVec);
	}

	PlayerObj& World::getPlayer()
	{
		return player;
	}

	const int& World::getRenderDistance()
	{
		return renderDistance;
	}

	void World::updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, Block::DIRECTION& direction, float& face, float& offset, float& closestPointLength)
	{
		const glm::vec3 eyeLocation = Camera::getEyeCoords();
		const glm::vec3 forwardVec = Camera::getForwardVec();

		const float length = glm::length2(eyeLocation - interceptPoint);
		if (length < selection_distance_blocks_square && length < closestPointLength)
		{
			if (Chunk::getType(pos) != Block::TYPE::AIR)
			{
				closestPointLength = length;
				selectedPos = pos;
				selectedFace = direction;
				blockFound = true;
				done = true;
			}
		}
		else
		{
			done = true;
		}

		face += offset;
	}

	// get the coords of selected block
	void World::selectBlock()
	{
		if (Camera::cameraMoved() || Camera::cameraRotated())
		{
			const glm::vec3 eyeLocation = Camera::getEyeCoords();
			const glm::vec3 forwardVec = Camera::getForwardVec();
			float closestPointLength = selection_distance_blocks_square;
			float factor;
			bool blockFound = false;

			Block::DIRECTION yzDirection, xzDirection, xyDirection;
			glm::vec3 
				yzInterceptPoint,
				xzInterceptPoint,
				xyInterceptPoint;
			glm::vec3
				yzPos = NOT_SELECTED,
				xzPos = NOT_SELECTED,
				xyPos = NOT_SELECTED;
			bool
				yzDone = forwardVec.x == 0,
				xzDone = forwardVec.y == 0,
				xyDone = forwardVec.z == 0;
			float yzFace, xzFace, xyFace, xOffset, yOffset, zOffset;
			bool
				hitWest = forwardVec.x > 0,
				hitSouth = forwardVec.y > 0,
				hitBottom = forwardVec.z > 0;

			if (hitWest)
			{
				yzFace = ceil(eyeLocation.x);
				xOffset = 1.0f;
			}
			else
			{
				yzFace = floor(eyeLocation.x);
				xOffset = -1.0f;
			}

			if (hitSouth)
			{
				xzFace = ceil(eyeLocation.y);
				yOffset = 1.0f;
			}
			else
			{
				xzFace = floor(eyeLocation.y);
				yOffset = -1.0f;
			}

			if (hitBottom)
			{
				xyFace = ceil(eyeLocation.z);
				zOffset = 1.0f;
			}
			else
			{
				xyFace = floor(eyeLocation.z);
				zOffset = -1.0f;
			}

			while (!xyDone || !yzDone || !xzDone)
			{
				if (!yzDone)
				{
					if (hitWest)
					{
						factor = abs((eyeLocation.x - yzFace) / forwardVec.x);
						yzInterceptPoint = { yzFace, factor * forwardVec.y + eyeLocation.y, factor * forwardVec.z + eyeLocation.z };
						yzPos = { yzInterceptPoint.x, floor(yzInterceptPoint.y), floor(yzInterceptPoint.z) };
						yzDirection = Block::DIRECTION::WEST;
					}
					else
					{
						factor = abs((eyeLocation.x - yzFace) / forwardVec.x);
						yzInterceptPoint = { yzFace, factor * forwardVec.y + eyeLocation.y, factor * forwardVec.z + eyeLocation.z };
						yzPos = { yzInterceptPoint.x - 1.0f, floor(yzInterceptPoint.y), floor(yzInterceptPoint.z) };
						yzDirection = Block::DIRECTION::EAST;
					}
					updateClosestPoint(blockFound, yzDone, yzInterceptPoint, yzPos, yzDirection, yzFace, xOffset, closestPointLength);
				}

				if (!xzDone)
				{
					if (hitSouth)
					{
						factor = abs((eyeLocation.y - xzFace) / forwardVec.y);
						xzInterceptPoint = { factor * forwardVec.x + eyeLocation.x, xzFace, factor * forwardVec.z + eyeLocation.z };
						xzPos = { floor(xzInterceptPoint.x), xzInterceptPoint.y, floor(xzInterceptPoint.z) };
						yzDirection = Block::DIRECTION::SOUTH;
					}
					else
					{
						factor = abs((eyeLocation.y - xzFace) / forwardVec.y);
						xzInterceptPoint = { factor * forwardVec.x + eyeLocation.x, xzFace, factor * forwardVec.z + eyeLocation.z };
						xzPos = { floor(xzInterceptPoint.x), xzInterceptPoint.y - 1.0f, floor(xzInterceptPoint.z) };
						yzDirection = Block::DIRECTION::NORTH;
					}
					updateClosestPoint(blockFound, xzDone, xzInterceptPoint, xzPos, xzDirection, xzFace, yOffset, closestPointLength);
				}

				if (!xyDone)
				{
					if (hitBottom)
					{
						factor = abs((eyeLocation.z - xyFace) / forwardVec.z);
						xyInterceptPoint = { factor * forwardVec.x + eyeLocation.x, factor * forwardVec.y + eyeLocation.y, xyFace };
						xyPos = { floor(xyInterceptPoint.x) , floor(xyInterceptPoint.y), xyInterceptPoint.z };
						yzDirection = Block::DIRECTION::BOTTOM;
					}
					else
					{
						factor = abs((eyeLocation.z - xyFace) / forwardVec.z);
						xyInterceptPoint = { factor * forwardVec.x + eyeLocation.x, factor * forwardVec.y + eyeLocation.y, xyFace };
						xyPos = { floor(xyInterceptPoint.x) , floor(xyInterceptPoint.y), xyInterceptPoint.z - 1.0f };
						yzDirection = Block::DIRECTION::TOP;
					}
					updateClosestPoint(blockFound, xyDone, xyInterceptPoint, xyPos, xyDirection, xyFace, zOffset, closestPointLength);
				}
			}

			if (blockFound)
				wireframe.setCoords(selectedPos);
			else
			{
				selectedPos = NOT_SELECTED;
				selectedFace = Block::DIRECTION::INVALID;
			}
		}
	}

	const glm::vec3& World::getSelectedBlockPos()
	{
		return selectedPos;
	}

	const Block::DIRECTION& World::getSelectedFace()
	{
		return selectedFace;
	}

	const glm::vec2 World::syncZone(const std::optional<glm::vec2>& zone = std::nullopt)
	{
		std::lock_guard<std::mutex> guard(zone_lock);

		if (zone)
		{
			bufferZone = *zone;
		}

		return bufferZone;
	}

	bool World::isOutOfWorld(glm::vec2& coord, glm::vec2& zone)
	{
		return
			coord.x < zone.x - renderDistanceNum  ||
			coord.x >= zone.x + renderDistanceNum ||
			coord.y < zone.y - renderDistanceNum  ||
			coord.y >= zone.y + renderDistanceNum;
	}

	void World::ReleaseChunks(std::vector<Chunk*>& to_be_released)
	{
		while (true)
		{
			release_lock.lock();
			int size = to_be_released.size();
			if (gameover && !size)
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

	// procedurally generate terrains in another thread
	void World::Generate()
	{
		std::vector<Chunk*> to_be_released;
		std::unordered_map<glm::vec2, Chunk*> to_be_added;
		std::vector<glm::vec2> to_be_created;
		bool initialized = false;
		glm::vec2 prevZone = syncZone();
		std::thread release_thread = std::thread(ReleaseChunks, std::ref(to_be_released));

		while (!gameover)
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
				const int chunkSize = current.size();
				for (int i = 0; i < chunkSize; i ++)
				{
					Chunk* chunk = current[i];
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
				for (int y = -renderDistance; y < renderDistance; y++)
				{
					for (int x = -renderDistance; x < renderDistance; x++)
					{
						glm::vec2 coord = glm::vec2{ x, y } * (float)CHUNK_WIDTH + currentZone;
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
				current.swap(work_in_progress);
				chunk_lock.unlock();
				release_lock.unlock();
				// copy current to work_in_progress
				work_in_progress = current;

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
			current.swap(work_in_progress);
			chunk_lock.unlock();
			work_in_progress = current;

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

	void World::Update()
	{
		// TODO: handle objects collision
		player.Begin(collisionEnabled, gravityEnabled);
		player.End(collisionEnabled, gravityEnabled);

		if (selectionEnabled)
		{
			selectBlock();
		}

		// update chunk buffer zone
		if (infiniteWorldEnabled)
		{
			glm::vec2 temp = bufferZone;
			const float
				rightEdge = bufferZone.x + zoneWidth,
				leftEdge = bufferZone.x - zoneWidth,
				topEdge = bufferZone.y + zoneWidth,
				bottomEdge = bufferZone.y - zoneWidth;

			if (
				Camera::getEyeCoords().x < leftEdge - zoneWidth ||
				Camera::getEyeCoords().x > rightEdge + zoneWidth ||
				Camera::getEyeCoords().y < bottomEdge - zoneWidth ||
				Camera::getEyeCoords().y > topEdge + zoneWidth
			)
			{
				temp = glm::vec2{ std::floor(Camera::getEyeCoords().x), std::floor(Camera::getEyeCoords().y) };
				syncZone(temp);
			}
			else
			{
				if (Camera::getEyeCoords().x > rightEdge)
				{
					temp.x = rightEdge;
					syncZone(temp);
				}
				else if (Camera::getEyeCoords().x < leftEdge)
				{
					temp.x = leftEdge;
					syncZone(temp);
				}

				if (Camera::getEyeCoords().y > topEdge)
				{
					temp.y = topEdge;
					syncZone(temp);
				}
				else if (Camera::getEyeCoords().y < bottomEdge)
				{
					temp.y = bottomEdge;
					syncZone(temp);
				}
			}
		}

		Draw();
	}

	void World::Draw()
	{
		// rendering pass 0
		if (BaseObj::getShowHitBox())
		{
			player.DrawHitBox();
		}

		if (selectionEnabled && selectedPos.z != -1.0f)
		{
			// TODO: find a better way to render the wireframe
			wireframe.Draw();
		}

		chunk_lock.lock();
		const int chunksNum = current.size();
		for (int i = 0; i < chunksNum; i++)
		{
			current[i]->Draw(Chunk::Phase::OPAQUE_P);
		}

		// rendering pass 1
		for (int i = 0; i < chunksNum; i++)
		{
			current[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < chunksNum; i++)
		{
			current[i]->Draw(Chunk::Phase::WATER_P);
		}

		// rendering pass 2
		Chunk::DrawTransparent();
		chunk_lock.unlock();
	}

	void World::Destroy()
	{
		terrain_generation_thread.join();
		for (std::vector<Chunk*>::iterator iter = work_in_progress.begin(); iter != work_in_progress.end(); ++iter)
		{
			delete (*iter);
		}
		std::vector<Chunk*>().swap(current);
		std::vector<Chunk*>().swap(work_in_progress);
	}
}
