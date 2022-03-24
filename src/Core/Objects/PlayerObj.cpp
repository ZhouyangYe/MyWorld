#include "PlayerObj.h"

namespace MyWorld
{
	// relative coordinates based on camera coords
	const glm::vec3 PlayerObj::posVec = { 0.4f, 0.4f, 1.8f };

	void PlayerObj::Update(bool& collisionEnabled, bool& gravityEnabled)
	{
		uint8_t hitX = 0, hitY = 0, hitZ = 0;

		// hook up with camera
		if (Camera::cameraMoved())
			hitBox.setPos(Camera::getEyeCoords() - posVec);

		if (gravityEnabled)
		{
			hitBox.applyGravity();
		}

		if (collisionEnabled)
		{
			handleTerrainCollision(hitX, hitY, hitZ);
		}

		if (gravityEnabled)
		{
			if (hitX != 0) hitBox.setVelocityX(0.0f);
			if (hitY != 0) hitBox.setVelocityX(0.0f);
			// if we are in the air, player move should be disabled
			if (hitZ == 0)
			{
				Camera::cameraMoveDisabled = true;
			}
			else
			{
				hitBox.setVelocityX(0.0f);
				if (hitZ == 1) Camera::cameraMoveDisabled = false;
			}
		}

		// correct the camera pos according to collision and gravity
		Camera::setCamPos(hitBox.getCoords() + posVec);

		if (showHitBox)
			hitBox.Draw();
	}
}
