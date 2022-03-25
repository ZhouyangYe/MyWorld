#include "BaseObj.h"

namespace MyWorld
{
	bool BaseObj::showHitBox = true;

	const bool& BaseObj::getShowHitBox()
	{
		return showHitBox;
	}

	void BaseObj::setShowHitBox(bool&& show)
	{
		showHitBox = show;
	}

	void BaseObj::setPos(glm::vec3& pos)
	{
		handler.setPrevPos(pos);
		hitBox.setPos(pos);
	}
}
