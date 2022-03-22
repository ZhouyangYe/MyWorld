#pragma once
#include "../../Physics/HitBox.h"
#include "../Chunk.h"

namespace MyWorld
{
	class BaseObj
	{
	protected:
		static bool showHitBox;
		HitBox hitBox;
		bool handleTerrainCollision();
	public:
		static void setShowHitBox(bool&& show);
		static const bool& getShowHitBox();
		BaseObj(HitBox&& hitBox) : hitBox(hitBox) {}
		virtual void Update(bool& collisionEnabled) = 0;
	};
}
