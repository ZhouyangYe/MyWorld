#pragma once
#include "BaseObj.h"
#include "../../Renderer/Camera.h"
#include "../../Renderer/Model/Characters/Player.h"

namespace MyWorld
{
	class PlayerObj : public BaseObj
	{
	public:
		static const glm::vec3 posVec;
		PlayerObj() : BaseObj({ false, 66.6f, glm::vec3{0.0f, 0.0f, 0.0f}, 0.8f, 2.0f }) {}
		void Update(bool& collisionEnabled, bool& gravityEnabled) override;
	};
}
