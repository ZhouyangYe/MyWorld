#include "PlayerObj.h"

namespace MyWorld
{
	// relative coordinates based on camera coords
	const glm::vec3 PlayerObj::posVec = { 0.4f, 0.4f, 1.8f };

	void PlayerObj::Update(bool& collisionEnabled)
	{
		// hook up with camera
		hitBox.setPos(Camera::getCameraCoords() - posVec);
		if (collisionEnabled && handleTerrainCollision())
		{
			// if there is a collision, correct the camera pos
			Camera::setCamPos(hitBox.getCoords() + posVec);
		}
		if (showHitBox)
			hitBox.Draw();
	}
}
