#include "World.h"

namespace MyWorld
{
	bool World::selectionEnabled = true;
	bool World::collisionEnabled = true;
	bool World::gravityEnabled = true;
	bool World::infiniteWorldEnabled = true;
	std::vector<Chunk*> World::chunks;
	int World::zoneChunkNum = 1;
	int World::zoneWidth = zoneChunkNum * CHUNK_WIDTH;
	float World::selection_distance_blocks = 5.0f;
	float World::selection_distance_blocks_square = selection_distance_blocks * selection_distance_blocks;
	Wireframe World::wireframe;
	PlayerObj World::player;
	const glm::vec3 World::NOT_SELECTED = { 0.0f, 0.0f, -1.0f };
	glm::vec3 World::selectedPos = NOT_SELECTED;
	Block::DIRECTION World::selectedFace = Block::DIRECTION::INVALID;

	void World::Init()
	{
		TerrainGeneration::setShowWorldBorder(!infiniteWorldEnabled);

		if (infiniteWorldEnabled)
		{
			chunks.reserve(4);
			for (int y = -zoneChunkNum; y < zoneChunkNum; y++)
			{
				for (int x = -zoneChunkNum; x < zoneChunkNum; x++)
				{
					chunks.push_back(new Chunk(glm::vec2{ (float)(x * CHUNK_WIDTH), (float)(y * CHUNK_WIDTH) }));
				}
			}

			// generate terrain in terrain generation thread
			ChunkGeneration::Start(chunks);
		}
		else
		{
			chunks.reserve(TerrainGeneration::ChunkNum());
			for (int y = -TerrainGeneration::ChunkRenderDistance(); y < TerrainGeneration::ChunkRenderDistance(); y++)
			{
				for (int x = -TerrainGeneration::ChunkRenderDistance(); x < TerrainGeneration::ChunkRenderDistance(); x++)
				{
					chunks.push_back(new Chunk(glm::vec2{ (float)(x * CHUNK_WIDTH), (float)(y * CHUNK_WIDTH) }));
				}
			}
		}

		player.setPos(Chunk::getSpawnLocation() - PlayerObj::posVec);
	}

	PlayerObj& World::getPlayer()
	{
		return player;
	}

	void World::updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, Block::DIRECTION& direction, float& face, float& offset, float& closestPointLength)
	{
		const glm::vec3 eyeLocation = Camera::getEyeCoords();
		const glm::vec3 forwardVec = Camera::getForwardVec();

		const float length = glm::length2(eyeLocation - interceptPoint);
		if (length < selection_distance_blocks_square && length < closestPointLength)
		{
			if (TerrainGeneration::getType(pos) != Block::TYPE::AIR)
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
						xzDirection = Block::DIRECTION::SOUTH;
					}
					else
					{
						factor = abs((eyeLocation.y - xzFace) / forwardVec.y);
						xzInterceptPoint = { factor * forwardVec.x + eyeLocation.x, xzFace, factor * forwardVec.z + eyeLocation.z };
						xzPos = { floor(xzInterceptPoint.x), xzInterceptPoint.y - 1.0f, floor(xzInterceptPoint.z) };
						xzDirection = Block::DIRECTION::NORTH;
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
						xyDirection = Block::DIRECTION::BOTTOM;
					}
					else
					{
						factor = abs((eyeLocation.z - xyFace) / forwardVec.z);
						xyInterceptPoint = { factor * forwardVec.x + eyeLocation.x, factor * forwardVec.y + eyeLocation.y, xyFace };
						xyPos = { floor(xyInterceptPoint.x) , floor(xyInterceptPoint.y), xyInterceptPoint.z - 1.0f };
						xyDirection = Block::DIRECTION::TOP;
					}
					updateClosestPoint(blockFound, xyDone, xyInterceptPoint, xyPos, xyDirection, xyFace, zOffset, closestPointLength);
				}
			}

			if (blockFound) {
				wireframe.setCoords(selectedPos);
			}
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
			glm::vec2 temp = ChunkGeneration::BufferZone();
			const float
				rightEdge = temp.x + zoneWidth,
				leftEdge = temp.x - zoneWidth,
				topEdge = temp.y + zoneWidth,
				bottomEdge = temp.y - zoneWidth;

			if (
				Camera::getEyeCoords().x < leftEdge - zoneWidth ||
				Camera::getEyeCoords().x > rightEdge + zoneWidth ||
				Camera::getEyeCoords().y < bottomEdge - zoneWidth ||
				Camera::getEyeCoords().y > topEdge + zoneWidth
			)
			{
				temp = glm::vec2{ std::floor(Camera::getEyeCoords().x), std::floor(Camera::getEyeCoords().y) };
				ChunkGeneration::syncZone(temp);
			}
			else
			{
				if (Camera::getEyeCoords().x > rightEdge)
				{
					temp.x = rightEdge;
					ChunkGeneration::syncZone(temp);
				}
				else if (Camera::getEyeCoords().x < leftEdge)
				{
					temp.x = leftEdge;
					ChunkGeneration::syncZone(temp);
				}

				if (Camera::getEyeCoords().y > topEdge)
				{
					temp.y = topEdge;
					ChunkGeneration::syncZone(temp);
				}
				else if (Camera::getEyeCoords().y < bottomEdge)
				{
					temp.y = bottomEdge;
					ChunkGeneration::syncZone(temp);
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

		// draw outline for selected block
		if (selectionEnabled && selectedPos.z != -1.0f)
		{
			wireframe.Draw();
		}

		ChunkGeneration::ChunkLock().lock();
		const int chunksNum = chunks.size();
		for (int i = 0; i < chunksNum; i++)
		{
			chunks[i]->Draw(Chunk::Phase::OPAQUE_P);
		}

		// rendering pass 1
		for (int i = 0; i < chunksNum; i++)
		{
			chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < chunksNum; i++)
		{
			chunks[i]->Draw(Chunk::Phase::WATER_P);
		}

		// rendering pass 2
		Chunk::DrawTransparent();
		ChunkGeneration::ChunkLock().unlock();
	}

	void World::Destroy()
	{
		ChunkGeneration::Terminate();
		std::vector<Chunk*>().swap(chunks);
	}
}
