#include "Info.h"

// display game info
namespace MyWorld
{
	bool Info::showInfo = false;

	void Info::setVisible(bool visible)
	{
		showInfo = visible;
	}

	void Info::Update()
	{
		// print fps
		if (showInfo)
		{
			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(1, 1, 0x0f, "\x1b[9;mFPS: %d"
				, Time::getFps()
			);
		}
	}
}
