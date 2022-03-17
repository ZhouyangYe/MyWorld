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

	void World::updateClosestPoint(bool& blockFound, float& closestPointLength, const Block::DIRECTION& direction, bool& outOfRange, int& face, const int& offset, const glm::vec3& forwardVec, const glm::vec3& eyeLocation)
	{
		glm::vec3 interceptPoint;
		glm::vec3 pos = NOT_SELECTED;
		float length;
		float factor1, factor2;

		switch (direction)
		{
		case Block::DIRECTION::EAST:
			factor1 = abs(face / forwardVec.x);
			factor2 = abs(eyeLocation.x / forwardVec.x);
			interceptPoint = { (float)face, (factor1 - factor2) * forwardVec.y + eyeLocation.y, (factor1 - factor2) * forwardVec.z + eyeLocation.z };
			pos = { interceptPoint.x - 1.0f, floor(interceptPoint.y), floor(interceptPoint.z) };
			break;
		case Block::DIRECTION::WEST:
			factor1 = abs(face / forwardVec.x);
			factor2 = abs(eyeLocation.x / forwardVec.x);
			interceptPoint = { (float)face, (factor1 - factor2) * forwardVec.y + eyeLocation.y, (factor1 - factor2) * forwardVec.z + eyeLocation.z };
			pos = { interceptPoint.x, floor(interceptPoint.y), floor(interceptPoint.z) };
			break;
		case Block::DIRECTION::NORTH:
			factor1 = abs(face / forwardVec.y);
			factor2 = abs(eyeLocation.y / forwardVec.y);
			interceptPoint = { (factor1 - factor2) * forwardVec.x + eyeLocation.x, (float)face, (factor1 - factor2) * forwardVec.z + eyeLocation.z };
			pos = { floor(interceptPoint.x), interceptPoint.y - 1.0f, floor(interceptPoint.z) };
			break;
		case Block::DIRECTION::SOUTH:
			factor1 = abs(face / forwardVec.y);
			factor2 = abs(eyeLocation.y / forwardVec.y);
			interceptPoint = { (factor1 - factor2) * forwardVec.x + eyeLocation.x, (float)face, (factor1 - factor2) * forwardVec.z + eyeLocation.z };
			pos = { floor(interceptPoint.x), interceptPoint.y, floor(interceptPoint.z) };
			break;
		case Block::DIRECTION::TOP:
			factor1 = abs(face / forwardVec.z);
			factor2 = abs(eyeLocation.z / forwardVec.z);
			interceptPoint = { (factor1 - factor2) * forwardVec.x + eyeLocation.x, (factor1 - factor2) * forwardVec.y + eyeLocation.y, (float)face };
			pos = { floor(interceptPoint.x) , floor(interceptPoint.y), interceptPoint.z - 1.0f };
			break;
		case Block::DIRECTION::BOTTOM:
			factor1 = abs(face / forwardVec.z);
			factor2 = abs(eyeLocation.z / forwardVec.z);
			interceptPoint = { (factor1 - factor2) * forwardVec.x + eyeLocation.x, (factor1 - factor2) * forwardVec.y + eyeLocation.y, (float)face };
			pos = { floor(interceptPoint.x) , floor(interceptPoint.y), interceptPoint.z };
			break;
		default:
			break;
		}

		length = glm::length2(eyeLocation - interceptPoint);
		Util::log(length, "length: ");
		Util::log(pos, "pos: ");
		Util::log(interceptPoint, "interceptPoint: ");
		Util::log(Chunk::getType(pos) != Block::AIR, "isNotAir: ");
		Util::log(direction, "direction: ");

		if (length < distance_blocks_square)
		{
			if (length < closestPointLength && Chunk::getType(pos) != Block::AIR)
			{
				Util::log("reach");
				closestPointLength = length;
				selectedPos = pos;
				blockFound = true;
			}
		}
		else
		{
			outOfRange = true;
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

			bool
				blockFound = false,
				yzOutOfRange = forwardVec.x == 0,
				xzOutOfRange = forwardVec.y == 0,
				xyOutOfRange = forwardVec.z == 0;

			int yzFace, xzFace, xyFace;

			int xOffset, yOffset, zOffset;

			Block::DIRECTION yzDirection, xzDirection, xyDirection;

			if (forwardVec.x > 0)
			{
				yzFace = ceil(eyeLocation.x);
				xOffset = 1;
				yzDirection = Block::DIRECTION::WEST;
			}
			else
			{
				yzFace = floor(eyeLocation.x);
				xOffset = -1;
				yzDirection = Block::DIRECTION::EAST;
			}

			if (forwardVec.y > 0)
			{
				xzFace = ceil(eyeLocation.y);
				yOffset = 1;
				xzDirection = Block::DIRECTION::SOUTH;
			}
			else
			{
				xzFace = floor(eyeLocation.y);
				yOffset = -1;
				xzDirection = Block::DIRECTION::NORTH;
			}

			if (forwardVec.z > 0)
			{
				xyFace = ceil(eyeLocation.z);
				zOffset = 1;
				xyDirection = Block::DIRECTION::BOTTOM;
			}
			else
			{
				xyFace = floor(eyeLocation.z);
				zOffset = -1;
				xyDirection = Block::DIRECTION::TOP;
			}

			Util::log("---------");
			while (!blockFound && (!xyOutOfRange || !yzOutOfRange || !yzOutOfRange))
			{
				if (!yzOutOfRange)
				{
					Util::log("in1");
					updateClosestPoint(blockFound, closestPointLength, yzDirection, yzOutOfRange, yzFace, xOffset, forwardVec, eyeLocation);
				}

				if (!xzOutOfRange)
				{
					Util::log("in2");
					updateClosestPoint(blockFound, closestPointLength, xzDirection, xzOutOfRange, xzFace, yOffset, forwardVec, eyeLocation);
				}

				if (!xyOutOfRange)
				{
					Util::log("in3");
					updateClosestPoint(blockFound, closestPointLength, xyDirection, xyOutOfRange, xyFace, zOffset, forwardVec, eyeLocation);
				}
			}

			if (blockFound)
			{
				wireframe->setCoords(selectedPos);
			}
			else
			{
				selectedPos = NOT_SELECTED;
			}
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
		if (selectedPos.z != -1.0f)
		{
			wireframe->Draw();
		}
	}

	void World::Destroy()
	{
		Data::Destroy();
	}
}
