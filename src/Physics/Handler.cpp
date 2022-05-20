#include "Handler.h"

namespace MyWorld
{
	void Handler::setPrevPos(glm::vec3& pos)
	{
		prevPos = pos;
	}

	void Handler::applyGravity()
	{
		if (hitBox->getVelocity().z < MAX_SPEED)
			hitBox->setVelocityZ(hitBox->getVelocity().z + GRAVITY);
		if (hitBox->getVelocity().z > MAX_SPEED)
			hitBox->setVelocityZ(MAX_SPEED);
	}

	void Handler::commitVelocity()
	{
		hitBox->updatePos();
	}

	bool&& Handler::notCollide(Block::TYPE type)
	{
		return type == Block::TYPE::AIR || type == Block::TYPE::WATER;
	}

	bool&& Handler::handleTerrainCollision(uint8_t& hitX, uint8_t& hitY, uint8_t& hitZ, Block::TYPE& hitType)
	{
		glm::vec3 pos = hitBox->getCoords();
		glm::vec3 offsetCoords{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 movement = pos - prevPos;

		// pre-calculate x
		if (movement.x != 0.0f)
		{
			bool done = false;
			glm::vec3 coords{ movement.x + prevPos.x, prevPos.y, prevPos.z };
			const float
				startX = floor(coords.x),
				startY = floor(coords.y),
				startZ = floor(coords.z),
				endX = coords.x + hitBox->width,
				endY = coords.y + hitBox->width,
				endZ = coords.z + hitBox->height;
			const float x = movement.x > 0.0f ? floor(endX) : startX;
			for (float y = startY; y < endY && !done; y++)
			{
				for (float z = startZ; z < endZ && !done; z++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					offsetCoords.x = hitBox->getCollisionOffset(HitBox::terrainHitBox).x;
					done = true;
				}
			}
		}

		// pre-calculate y
		if (movement.y != 0.0f)
		{
			bool done = false;
			glm::vec3 coords{ prevPos.x, movement.y + prevPos.y, prevPos.z };
			const float
				startX = floor(coords.x),
				startY = floor(coords.y),
				startZ = floor(coords.z),
				endX = coords.x + hitBox->width,
				endY = coords.y + hitBox->width,
				endZ = coords.z + hitBox->height;
			const float y = movement.y > 0.0f ? floor(endY) : startY;
			for (float x = startX; x < endX && !done; x++)
			{
				for (float z = startZ; z < endZ && !done; z++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					offsetCoords.y = hitBox->getCollisionOffset(HitBox::terrainHitBox).y;
					done = true;
				}
			}
		}

		// pre-calculate z
		if (movement.z != 0.0f)
		{
			bool done = false;
			glm::vec3 coords{ prevPos.x, prevPos.y, movement.z + prevPos.z };
			const float
				startX = floor(coords.x),
				startY = floor(coords.y),
				startZ = floor(coords.z),
				endX = coords.x + hitBox->width,
				endY = coords.y + hitBox->width,
				endZ = coords.z + hitBox->height;
			const float z = movement.z > 0.0f ? floor(endZ) : startZ;
			for (float x = startX; x < endX && !done; x++)
			{
				for (float y = startY; y < endY && !done; y++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					offsetCoords.z = hitBox->getCollisionOffset(HitBox::terrainHitBox).z;
					done = true;
				}
			}
		}

		if (offsetCoords.x == 0.0f && offsetCoords.y == 0.0f && offsetCoords.z == 0.0f)
		{
			// pre-calculate x, y
			if (movement.x != 0.0f && movement.y != 0.0f)
			{
				bool done = false;
				glm::vec3 coords{ movement.x + prevPos.x, movement.y + prevPos.y, prevPos.z };
				const float
					startX = floor(coords.x),
					startY = floor(coords.y),
					startZ = floor(coords.z),
					endX = coords.x + hitBox->width,
					endY = coords.y + hitBox->width,
					endZ = coords.z + hitBox->height;
				const float x = movement.x > 0.0f ? floor(endX) : startX;
				const float y = movement.y > 0.0f ? floor(endY) : startY;
				for (float z = startZ; z < endZ && !done; z++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					glm::vec3 offsets = hitBox->getCollisionOffset(HitBox::terrainHitBox);
					if (abs(offsets.x) < abs(offsets.y))
						offsetCoords.x = offsets.x;
					else
						offsetCoords.y = offsets.y;
					done = true;
				}
			}

			// pre-calculate x, z
			if (movement.x != 0.0f && movement.z != 0.0f)
			{
				bool done = false;
				glm::vec3 coords{ movement.x + prevPos.x, prevPos.y, movement.z + prevPos.z };
				const float
					startX = floor(coords.x),
					startY = floor(coords.y),
					startZ = floor(coords.z),
					endX = coords.x + hitBox->width,
					endY = coords.y + hitBox->width,
					endZ = coords.z + hitBox->height;
				const float x = movement.x > 0.0f ? floor(endX) : startX;
				const float z = movement.z > 0.0f ? floor(endZ) : startZ;
				for (float y = startY; y < endY && !done; y++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					glm::vec3 offsets = hitBox->getCollisionOffset(HitBox::terrainHitBox);
					if (abs(offsets.x) < abs(offsets.z))
						offsetCoords.x = offsets.x;
					else
						offsetCoords.z = offsets.z;
					done = true;
				}
			}

			// pre-calculate y, z
			if (movement.y != 0.0f && movement.z != 0.0f)
			{
				bool done = false;
				glm::vec3 coords{ prevPos.x, movement.y + prevPos.y, movement.z + prevPos.z };
				const float
					startX = floor(coords.x),
					startY = floor(coords.y),
					startZ = floor(coords.z),
					endX = coords.x + hitBox->width,
					endY = coords.y + hitBox->width,
					endZ = coords.z + hitBox->height;
				const float y = movement.y > 0.0f ? floor(endY) : startY;
				const float z = movement.z > 0.0f ? floor(endZ) : startZ;
				for (float x = startX; x < endX && !done; x++)
				{
					glm::vec3 blockCoords{ x, y, z };
					if (notCollide(TerrainGeneration::getType(blockCoords))) continue;

					HitBox::terrainHitBox.setPos(blockCoords);
					hitBox->setPos(coords);
					glm::vec3 offsets = hitBox->getCollisionOffset(HitBox::terrainHitBox);
					if (abs(offsets.y) < abs(offsets.z))
						offsetCoords.y = offsets.y;
					else
						offsetCoords.z = offsets.z;
					done = true;
				}
			}
		}

		// pre-calculate x, y, z
		if (offsetCoords.x == 0.0f && offsetCoords.y == 0.0f && offsetCoords.z == 0.0f && movement.x != 0.0f && movement.y != 0.0f && movement.z != 0.0f)
		{
			const float x = movement.x > 0.0f ? floor(pos.x + hitBox->width) : floor(pos.x);
			const float y = movement.y > 0.0f ? floor(pos.y + hitBox->width) : floor(pos.y);
			const float z = movement.z > 0.0f ? floor(pos.z + hitBox->height) : floor(pos.z);
			glm::vec3 blockCoords{ x, y, z };
			if (!notCollide(TerrainGeneration::getType(blockCoords)))
			{
				HitBox::terrainHitBox.setPos(blockCoords);
				hitBox->setPos(pos);
				glm::vec3 offsets = hitBox->getCollisionOffset(HitBox::terrainHitBox);
				float smallest = offsets.x;
				offsetCoords.x = smallest;
				if (abs(offsets.y) < abs(smallest))
				{
					smallest = offsets.y;
					offsetCoords.x = 0.0f;
					offsetCoords.y = smallest;
				}
				if (abs(offsets.z) < abs(smallest))
				{
					smallest = offsets.z;
					offsetCoords.y = 0.0f;
					offsetCoords.z = smallest;
				}
			}
		}

		if (offsetCoords.x == 0.0f && offsetCoords.y == 0.0f && offsetCoords.z == 0.0f)
		{
			hitBox->setPos(pos);
			prevPos = pos;
			return false;
		}

		if (offsetCoords.x != 0.0f) hitX = offsetCoords.x > 0.0f ? 1 : 2;
		if (offsetCoords.y != 0.0f) hitY = offsetCoords.y > 0.0f ? 1 : 2;
		if (offsetCoords.z != 0.0f) hitZ = offsetCoords.z > 0.0f ? 1 : 2;
		hitBox->setPos(pos + offsetCoords);
		prevPos = hitBox->getCoords();
		return true;
	}
}
