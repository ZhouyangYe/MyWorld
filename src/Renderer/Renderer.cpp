#include "Renderer.h"

namespace MyWorld
{
    static bx::DefaultAllocator s_allocator;
    static bx::AllocatorI* g_allocator = &s_allocator;
    typedef bx::StringT<&g_allocator> String;

    static String s_currentDir;

    class FileReader : public bx::FileReader
    {
        typedef bx::FileReader super;

    public:
        bx::DefaultAllocator g_allocator;
        virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
        {
            String filePath(s_currentDir);
            filePath.append(_filePath);
            return super::open(filePath.getPtr(), _err);
        }
    };

    bgfx::ProgramHandle Renderer::program = {};
    bgfx::VertexLayout Renderer::colorLayout;
    bgfx::VertexLayout Renderer::textureLayout;
    bgfx::VertexLayout Renderer::colorTextureLayout;

    const bgfx::Memory* Renderer::loadMem(bx::FileReaderI* _reader, const char* _filePath)
    {
        if (bx::open(_reader, _filePath))
        {
            uint32_t size = (uint32_t)bx::getSize(_reader);
            const bgfx::Memory* mem = bgfx::alloc(size + 1);
            bx::read(_reader, mem->data, size, bx::ErrorAssert{});
            bx::close(_reader);
            mem->data[mem->size - 1] = '\0';
            return mem;
        }

        DBG("Failed to load %s.", _filePath);
        return NULL;
    }

    bgfx::ShaderHandle Renderer::loadShader(const char* _name)
    {
        bx::FileReaderI* _reader = BX_NEW(g_allocator, FileReader);

        char filePath[512];

        const char* shaderPath = "???";

        //dx11/  dx9/   essl/  glsl/  metal/ pssl/  spirv/
        bgfx::ShaderHandle invalid = BGFX_INVALID_HANDLE;
        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D9:     shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/dx9/";   break;
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:    shaderPath = "c:\\Bright\\dev\\MyWorld\\libs\\bgfx.cmake\\bgfx\\examples\\runtime\\shaders\\dx11\\";  break;
            case bgfx::RendererType::Gnm:           shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/pssl/";  break;
            case bgfx::RendererType::Metal:         shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/metal/"; break;
            case bgfx::RendererType::OpenGL:        shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/glsl/";  break;
            case bgfx::RendererType::OpenGLES:      shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/essl/";  break;
            case bgfx::RendererType::Vulkan:        shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/spirv/"; break;
            case bgfx::RendererType::Nvn:
            case bgfx::RendererType::WebGPU:
            case bgfx::RendererType::Count:         return invalid; // count included to keep compiler warnings happy
        }

        bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
        bx::strCat(filePath, BX_COUNTOF(filePath), _name);
        bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

        bgfx::ShaderHandle handle = bgfx::createShader(loadMem(_reader, filePath));
        bgfx::setName(handle, _name);

        return handle;
    }

    bgfx::ProgramHandle& Renderer::getProgramHandle()
    {
        return program;
    }

    bgfx::VertexLayout& Renderer::getColorLayout()
    {
        return colorLayout;
    }

    bgfx::VertexLayout& Renderer::getTextureLayout()
    {
        return textureLayout;
    }

    bgfx::VertexLayout& Renderer::getColorTextureLayout()
    {
        return colorTextureLayout;
    }

    void Renderer::Init(RenderParam param)
    {
        bgfx::PlatformData pd;
        pd.ndt = NULL;
        pd.nwh = param.window;

        bgfx::Init bgfxInit;
        bgfxInit.type = bgfx::RendererType::Count; // pick one!

        // seems to default to vulkan which is fine by me!
        //bgfxInit.type = bgfx::RendererType::Vulkan;
        //bgfxInit.type = bgfx::RendererType::OpenGL;
        WindowSize windowSize = param.windowSize;
        bgfxInit.resolution.width = windowSize.width;
        bgfxInit.resolution.height = windowSize.height;
        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;

        // seems bgfx is bright enough to not use the active but unused integrated device!
        //bgfxInit.vendorId = BGFX_PCI_ID_NVIDIA; // just in case its selecting unused integrated device?

        bgfxInit.platformData = pd;
        bgfx::init(bgfxInit);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, windowSize.width, windowSize.height);

        bgfx::ShaderHandle vsh = loadShader("vs_cubes");
        printf("shader handle %i created for vs_cubes.bin\n", vsh.idx);
        if (vsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return;
        }

        bgfx::ShaderHandle fsh = loadShader("fs_cubes");
        printf("shader handle %i created for fs_cubes.bin \n", fsh.idx);
        if (fsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return;
        }

        program = bgfx::createProgram(vsh, fsh, true);
        printf("program handle %i created\n", program.idx);

        colorLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
        textureLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
        colorTextureLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    void Renderer::Terminate()
    {
        bgfx::destroy(program);
        bgfx::shutdown();
    }

    void Renderer::Begin()
    {

    }

    void Renderer::End()
    {
        bgfx::frame();
    }
}