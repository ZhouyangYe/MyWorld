#pragma once
#include <iostream>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bx/math.h>
#include <bimg/decode.h>

#include "Core/Util.h"

#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

#define BT_DEBUG true
#if BT_DEBUG
#define BX_ALLOC(_allocator, _size) bx::alloc(_allocator, _size, 0, __FILE__, __LINE__)
#define BX_FREE(_allocator, _ptr) bx::free(_allocator, _ptr, 0, __FILE__, __LINE__)
#else
#define BX_ALLOC(_allocator, _size) bx::alloc(_allocator, _size, 0)
#define BX_FREE(_allocator, _ptr) bx::free(_allocator, _ptr, 0)
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
		static bx::FileReaderI* _reader;
		static bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation);
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
		static bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
	};
}
