#include "World.h"

namespace MyWorld
{
	int World::chunk_num;
	float World::distance_blocks = 5.0f;
	float World::distance_blocks_square = distance_blocks * distance_blocks;
	Wireframe* World::wireframe = nullptr;
	const glm::vec3 World::NOT_SELECTED = { 0.0f, 0.0f, -1.0f };
	glm::vec3 World::selectedPos = NOT_SELECTED;

	void World::Generate()
	{
		Data::Init();
		wireframe = new Wireframe(NOT_SELECTED, glm::vec3{0.0f, 0.0f, 0.0f});

		const int num = Chunk::getChunkRenderDistanceNum();
		World::chunk_num = num * num * 4;
	}

	void World::updateClosestPoint(bool& blockFound, bool& done, glm::vec3& interceptPoint, glm::vec3& pos, float& face, float& offset, float& closestPointLength)
	{
		const glm::vec3 eyeLocation = Camera::getCameraCoords();
		const glm::vec3 forwardVec = Camera::getForwardVec();

		const float length = glm::length2(eyeLocation - interceptPoint);
		if (length < distance_blocks_square && length < closestPointLength)
		{
			if (Chunk::getType(pos) != Block::AIR)
			{
				closestPointLength = length;
				selectedPos = pos;
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
					}
					else
					{
						factor = abs((eyeLocation.x - yzFace) / forwardVec.x);
						yzInterceptPoint = { yzFace, factor * forwardVec.y + eyeLocation.y, factor * forwardVec.z + eyeLocation.z };
						yzPos = { yzInterceptPoint.x - 1.0f, floor(yzInterceptPoint.y), floor(yzInterceptPoint.z) };
					}
					updateClosestPoint(blockFound, yzDone, yzInterceptPoint, yzPos, yzFace, xOffset, closestPointLength);
				}

				if (!xzDone)
				{
					if (hitSouth)
					{
						factor = abs((eyeLocation.y - xzFace) / forwardVec.y);
						xzInterceptPoint = { factor * forwardVec.x + eyeLocation.x, xzFace, factor * forwardVec.z + eyeLocation.z };
						xzPos = { floor(xzInterceptPoint.x), xzInterceptPoint.y, floor(xzInterceptPoint.z) };
					}
					else
					{
						factor = abs((eyeLocation.y - xzFace) / forwardVec.y);
						xzInterceptPoint = { factor * forwardVec.x + eyeLocation.x, xzFace, factor * forwardVec.z + eyeLocation.z };
						xzPos = { floor(xzInterceptPoint.x), xzInterceptPoint.y - 1.0f, floor(xzInterceptPoint.z) };
					}
					updateClosestPoint(blockFound, xzDone, xzInterceptPoint, xzPos, xzFace, yOffset, closestPointLength);
				}

				if (!xyDone)
				{
					if (hitBottom)
					{
						factor = abs((eyeLocation.z - xyFace) / forwardVec.z);
						xyInterceptPoint = { factor * forwardVec.x + eyeLocation.x, factor * forwardVec.y + eyeLocation.y, xyFace };
						xyPos = { floor(xyInterceptPoint.x) , floor(xyInterceptPoint.y), xyInterceptPoint.z };
					}
					else
					{
						factor = abs((eyeLocation.z - xyFace) / forwardVec.z);
						xyInterceptPoint = { factor * forwardVec.x + eyeLocation.x, factor * forwardVec.y + eyeLocation.y, xyFace };
						xyPos = { floor(xyInterceptPoint.x) , floor(xyInterceptPoint.y), xyInterceptPoint.z - 1.0f };
					}
					updateClosestPoint(blockFound, xyDone, xyInterceptPoint, xyPos, xyFace, zOffset, closestPointLength);
				}
			}

			if (blockFound)
				wireframe->setCoords(selectedPos);
			else
				selectedPos = NOT_SELECTED;
		}
	}

	void World::Update()
	{
		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::OPAQUE_P);
		}

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_PLACEHOLDER_P);
		}

		for (int i = 0; i < chunk_num; i++)
		{
		  Data::chunks[i]->Draw(Chunk::Phase::WATER_P);
		}

		Chunk::DrawTransparent();

		selectBlock();
		if (selectedPos.z != -1.0f) wireframe->Draw();
	}

	void World::Destroy()
	{
		Data::Destroy();
	}
}
