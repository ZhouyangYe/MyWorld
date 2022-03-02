#pragma once
#include "Tools.h"


namespace MyWorld
{
	class Renderer
	{
	private:
		struct WindowSize
		{
			int width, height;
		};
		static bgfx::VertexLayout colorLayout;
		static bgfx::VertexLayout textureLayout;
		static bgfx::VertexLayout textureArrayLayout;
		static bgfx::VertexLayout colorTextureLayout;
		static bgfx::VertexLayout colorTextureArrayLayout;
		static uint32_t debugMode;
	public:
		struct RenderParam
		{
			void* window = nullptr;
			WindowSize windowSize;
		};
		static bgfx::ProgramHandle texture_program;
		static bgfx::ProgramHandle texture_color_program;
		static bgfx::ProgramHandle texture_array_program;
		static bgfx::ProgramHandle texture_array_color_program;
		static void Init(RenderParam param);
		static void Terminate();
		static void Begin();
		static void End();
		static void switchRenderMode();
		static bool toggleDebugText();
		static const bgfx::VertexLayout& getColorLayout();
		static const bgfx::VertexLayout& getTextureLayout();
		static const bgfx::VertexLayout& getTextureArrayLayout();
		static const bgfx::VertexLayout& getColorTextureLayout();
		static const bgfx::VertexLayout& getColorTextureArrayLayout();
		static const uint32_t& getDebugMode();
	};
}
