#pragma once
#include <iostream>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bx/math.h>

#include "Core/Util.h"

#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

#if BT_DEBUG
#define BX_ALLOC(_allocator, _size) bx::alloc(_allocator, _size, 0, __FILE__, __LINE__)
#else
#define BX_ALLOC(_allocator, _size) bx::alloc(_allocator, _size, 0)
#endif
#define BX_PLACEMENT_NEW(_ptr, _type) ::new(bx::PlacementNewTag(), _ptr) _type
#define BX_NEW(_allocator, _type) BX_PLACEMENT_NEW(BX_ALLOC(_allocator, sizeof(_type) ), _type)

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
		static bgfx::VertexLayout colorLayout;
		static bgfx::VertexLayout textureLayout;
		static bgfx::VertexLayout colorTextureLayout;
		static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath);
	public:
		struct RenderParam
		{
			void* window = nullptr;
			WindowSize windowSize;
		};
		static void Init(RenderParam param);
		static void Terminate();
		static void Begin();
		static void End();
		static bgfx::ProgramHandle& getProgramHandle();
		static bgfx::VertexLayout& getColorLayout();
		static bgfx::VertexLayout& getTextureLayout();
		static bgfx::VertexLayout& getColorTextureLayout();
	};
}
