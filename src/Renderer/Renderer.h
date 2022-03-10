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
		static uint32_t debugMode;
	public:
		struct PosColorVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;

			static bgfx::VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.end();
			};
		};
		struct PosTextureVertex
		{
			float x;
			float y;
			float z;
			float u;
			float v;

			static bgfx::VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();;
			};
		};
		struct PosTextureArrayVertex
		{
			float x;
			float y;
			float z;
			float u;
			float v;
			float d;

			static bgfx::VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
					.end();
			};
		};
		struct PosColorTextureVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
			float u;
			float v;

			static bgfx::VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();
			};
		};
		struct PosColorTextureArrayVertex
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
			float u;
			float v;
			float d;

			static bgfx::VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
					.end();
			};
		};
		struct RenderParam
		{
			void* window = nullptr;
			WindowSize windowSize;
		};
		static bgfx::ProgramHandle water_program;
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
		static const uint32_t& getDebugMode();
	};
}
