#pragma once
#include "Renderer.h"

namespace MyWorld
{
	class Camera
	{
	private:
		struct WindowSize
		{
			int width, height;
		};
		static float view[16];
		static float proj[16];
	public:
		struct CameraParam
		{
			WindowSize windowSize;
		};
		static void Init(CameraParam param);
		static void Terminate();
		static void Begin();
		static void End();
	};
}
