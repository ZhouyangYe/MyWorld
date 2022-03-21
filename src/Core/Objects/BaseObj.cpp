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
}
