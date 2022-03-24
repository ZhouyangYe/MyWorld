#pragma once
#include "../../Physics/HitBox.h"
#include "../Chunk.h"

namespace MyWorld
{
	class BaseObj
	{
	protected:
		static bool showHitBox; // SETTINGS
		glm::vec3 prevPos;
		HitBox hitBox;
		bool&& handleTerrainCollision(uint8_t& hitX, uint8_t& hitY, uint8_t& hitZ);
		bool&& notCollide(Block::TYPE type);
	public:
		void setPos(glm::vec3& pos);
		static void setShowHitBox(bool&& show);
		static const bool& getShowHitBox();
		BaseObj(HitBox&& hitBox) : hitBox(hitBox), prevPos(hitBox.getCoords()) {}
		virtual void Update(bool& collisionEnabled, bool& gravityEnabled) = 0;
	};
}
