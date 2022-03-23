#include "World.h"

namespace MyWorld
{
	bool World::selectionEnabled = true;
	bool World::collisionEnabled = true;
	bool World::infiniteWorldEnabled = false;
	int World::renderDistance = 1;
	int World::chunk_num;
	float World::distance_blocks = 5.0f;
	float World::distance_blocks_square = distance_blocks * distance_blocks;
	Wireframe World::wireframe;
	PlayerObj World::player;
	const glm::vec3 World::NOT_SELECTED = { 0.0f, 0.0f, -1.0f };
	glm::vec3 World::selectedPos = NOT_SELECTED;
	Block::DIRECTION World::selectedFace = Block::DIRECTION::INVALID;

	void World::Generate()
	{
		Data::Init({ infiniteWorldEnabled, renderDistance });

		World::chunk_num = renderDistance * renderDistance * 4;
	}

	void World::setSelectionEnabled(bool enabled)
	{
		selectionEnabled = enabled;
	}

	void World::setCollisionEnabled(bool enabled)
	{
		collisionEnabled = enabled;
	}

	void World::setInfiniteWorldEnabled(bool enabled)
	{
		infiniteWorldEnabled = enabled;
	}

	const int& World::getRenderDistance()
	{
		return renderDistance;
	}

	void World::updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, Block::DIRECTION& direction, float& face, float& offset, float& closestPointLength)
	{
		const glm::vec3 eyeLocation = Camera::getCameraCoords();
		const glm::vec3 forwardVec = Camera::getForwardVec();

		const float length = glm::length2(eyeLocation - interceptPoint);
		if (length < distance_blocks_square && length < closestPointLength)
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
			const glm::vec3 eyeLocation = Camera::getCameraCoords();
			const glm::vec3 forwardVec = Camera::getForwardVec();
			float closestPointLength = distance_blocks_square;
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

	void World::Update()
	{
		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::OPAQUE_P);
		}

		player.Update(collisionEnabled);

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_P);
		}

		Chunk::DrawTransparent();

		if (selectionEnabled)
		{
			selectBlock();
			if (selectedPos.z != -1.0f) wireframe.Draw();
		}
	}

	void World::Destroy()
	{
		Data::Destroy();
	}
}
