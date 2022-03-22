#include "BaseObj.h"

namespace MyWorld
{
	bool BaseObj::showHitBox = true;

	const bool& BaseObj::getShowHitBox()
	{
		return showHitBox;
	}

	void BaseObj::setShowHitBox(bool&& show)
	{
		showHitBox = show;
	}

	bool BaseObj::handleTerrainCollision()
	{
		const glm::vec3 pos = hitBox.getCoords();
		const float endX = pos.x + hitBox.width;
		const float endY = pos.y + hitBox.width;
		const float endZ = pos.z + hitBox.height;
		glm::vec3 offsetCoords = glm::vec3{ 0.0f, 0.0f, 0.0f };
		for (float x = floor(pos.x); x < endX; x++)
		{
			for (float y = floor(pos.y); y < endY; y++)
			{
				for (float z = floor(pos.z); z < endZ; z++)
				{
					glm::vec3 blockCoords{ x, y, z };
					// TODO: skip the inner blocks and only check the surface blocks?
					if (Chunk::getType(blockCoords) == Block::TYPE::AIR)
						continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					glm::vec3 offsets = hitBox.getCollisionOffset(HitBox::terrainHitBox);
					const float
						offsetX = abs(offsets.x),
						offsetY = abs(offsets.y),
						offsetZ = abs(offsets.z);
					glm::vec3 offset = glm::vec3{
						offsetX < offsetY && offsetX < offsetZ ? offsets.x : 0.0f,
						offsetY < offsetX && offsetY < offsetZ ? offsets.y : 0.0f,
						offsetZ < offsetX && offsetZ < offsetY ? offsets.z : 0.0f
					};
					offsetCoords = glm::vec3{
						abs(offsetCoords.x) > abs(offset.x) ? offsetCoords.x : offset.x,
						abs(offsetCoords.y) > abs(offset.y) ? offsetCoords.y : offset.y,
						abs(offsetCoords.z) > abs(offset.z) ? offsetCoords.z : offset.z
					};
				}
			}
		}

		if (offsetCoords.x == 0.0f && offsetCoords.y == 0.0f && offsetCoords.z == 0.0f) return false;

		hitBox.setPos(pos + offsetCoords);
		return true;
	}
}
