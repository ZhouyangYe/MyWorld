#pragma once
#include "../../Physics/HitBox.h"
#include "../Chunk.h"

namespace MyWorld
{
	class BaseObj
	{
	protected:
		glm::vec3 prevPos;
		static bool showHitBox;
		HitBox hitBox;
		bool handleTerrainCollision();
		bool hasCollision(Block::TYPE&& type);
	public:
		static void setShowHitBox(bool&& show);
		static const bool& getShowHitBox();
		BaseObj(HitBox&& hitBox) : hitBox(hitBox), prevPos(hitBox.getCoords()) {}
		virtual void Update(bool& collisionEnabled) = 0;
	};
}
