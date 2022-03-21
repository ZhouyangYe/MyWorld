#include "PlayerObj.h"

namespace MyWorld
{
	const glm::vec3 PlayerObj::posVec = { 0.4f, 0.4f, 1.8f };

	void PlayerObj::handleTerrainCollision()
	{
		const glm::vec3 pos = hitBox.getCoords();
		const float endX = pos.x + hitBox.width;
		const float endY = pos.y + hitBox.width;
		const float endZ = pos.z + hitBox.height;
		for (float x = floor(pos.x); x < endX; x++)
		{
			for (float y = floor(pos.y); y < endY; y++)
			{
				for (float z = floor(pos.z); z < endZ; z++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (Chunk::getType(blockCoords) == Block::TYPE::AIR)
						continue;
					HitBox::terrainHitBox.setPos(blockCoords);
					glm::vec3 offsets = hitBox.getCollisionOffset(HitBox::terrainHitBox);
					const float
						offsetX = abs(offsets.x),
						offsetY = abs(offsets.y),
						offsetZ = abs(offsets.z);
					glm::vec3 offsetCoords = glm::vec3{
						offsetX < offsetY && offsetX < offsetZ ? offsets.x : 0.0f,
						offsetY < offsetX && offsetY < offsetZ ? offsets.y : 0.0f,
						offsetZ < offsetX && offsetZ < offsetY ? offsets.z : 0.0f
					};
					glm::vec3 newCoords = pos + offsetCoords;
					hitBox.setPos(newCoords);
					Camera::setCamPos(newCoords + posVec);
				}
			}
		}
	}

	void PlayerObj::Update(bool& collisionEnabled)
	{
		glm::vec3 camCoords = Camera::getCameraCoords();
		// hook up with camera
		hitBox.setPos(camCoords - posVec);
		if (showHitBox)
			hitBox.Draw();
		if (collisionEnabled)
			handleTerrainCollision();
	}
}
