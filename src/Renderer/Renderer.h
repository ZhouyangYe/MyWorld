#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include "Core/Util.h"

namespace MyWorld
{
	class Renderer
	{
	private:
		struct WindowSize
		{
			int width, height;
		};
		static bgfx::ShaderHandle loadShader(const char* fileName);
		static bgfx::ProgramHandle program;
		static bgfx::VertexLayout vLayout;
	public:
		struct RenderParam
		{
			void* window = nullptr;
			WindowSize windowSize;
		};
		static void Init(RenderParam param);
		static void Terminate();
		static bgfx::ProgramHandle& getProgramHandle();
		static bgfx::VertexLayout& getVertexLayout();
	};
}
