#pragma once
#include "../../Physics/HitBox.h"
#include "../../Physics/Handler.h"

namespace MyWorld
{
	class BaseObj
	{
	protected:
		static bool showHitBox; // SETTINGS
		HitBox hitBox;   // hitBox
		Handler handler; // physics handler
	public:
		void setPos(glm::vec3& pos);
		static void setShowHitBox(bool&& show);
		static const bool& getShowHitBox();
		BaseObj(HitBox&& box) : hitBox(box), handler(&hitBox) {}
		virtual void Update(bool& collisionEnabled, bool& gravityEnabled) = 0;
	};
}
