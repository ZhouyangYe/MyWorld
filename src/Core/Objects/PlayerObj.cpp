#include "PlayerObj.h"

namespace MyWorld
{
	// relative coordinates based on camera coords
	const glm::vec3 PlayerObj::posVec = { 0.4f, 0.4f, 1.8f };

	void PlayerObj::clearVelocity()
	{
		if (hitBox.getVelocity().x == 0.0f && hitBox.getVelocity().y == 0.0f && hitBox.getVelocity().z == 0.0f) return;
		hitBox.setVelocity(0.0f, 0.0f, 0.0f);
	}

	void PlayerObj::Begin(bool& collisionEnabled, bool& gravityEnabled)
	{
		// hook up with camera
		if (Camera::cameraMoved())
			hitBox.setPos(Camera::getEyeCoords() - posVec);

		if (gravityEnabled)
		{
			handler.applyGravity();
		}

		handler.commitVelocity();
	}

	void PlayerObj::End(bool& collisionEnabled, bool& gravityEnabled)
	{
		uint8_t hitX = 0, hitY = 0, hitZ = 0;
		Block::TYPE hitType = Block::TYPE::AIR;

		if (collisionEnabled)
		{
			handler.handleTerrainCollision(hitX, hitY, hitZ, hitType);
		}

		if (gravityEnabled)
		{
			if (hitX != 0) hitBox.setVelocityX(0.0f);
			if (hitY != 0) hitBox.setVelocityY(0.0f);
			// if the player is in the air, player move should be disabled
			if (hitZ == 0)
			{
				if (!Camera::cameraMoveDisabled) Camera::cameraMoveDisabled = true;
			}
			else
			{
				hitBox.setVelocityZ(0.0f);
				if (hitZ == 1) Camera::cameraMoveDisabled = false;
			}
		}

		// correct the camera pos according to collision and gravity
		Camera::setCamPos(hitBox.getCoords() + posVec);
	}

	void PlayerObj::DrawHitBox()
	{
		hitBox.Draw();
	}
}
