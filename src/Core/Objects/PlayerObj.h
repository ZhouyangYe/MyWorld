#pragma once
#include "BaseObj.h"
#include "../../Renderer/Camera.h"
#include "../../Renderer/Model/Characters/Player.h"

namespace MyWorld
{
	class PlayerObj : BaseObj
	{
	private:
		static const glm::vec3 posVec;
		void handleTerrainCollision();
	public:
		PlayerObj() : BaseObj({ false, 66.6f, glm::vec3{0.0f, 0.0f, 193.0f}, 0.8f, 2.0f }) {}
		void Update(bool& collisionEnabled) override;
	};
}
