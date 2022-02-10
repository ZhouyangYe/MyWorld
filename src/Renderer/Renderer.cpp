#include "Renderer.h"

namespace MyWorld
{
    bgfx::ProgramHandle Renderer::program = {};
    bgfx::VertexLayout Renderer::vLayout;

    bgfx::ShaderHandle Renderer::loadShader(const char* fileName)
    {
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

        size_t shaderLen = strlen(shaderPath);
        size_t fileLen = strlen(fileName);
        char* filePath = (char*)malloc(shaderLen + fileLen + 1);
        memcpy(filePath, shaderPath, shaderLen);
        memcpy(&filePath[shaderLen], fileName, fileLen);
        filePath[shaderLen + fileLen] = 0;    // properly null terminate
        FILE* file = fopen(filePath, "rb");

        if (!file) {
            return invalid;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
        fread(mem->data, 1, fileSize, file);
        mem->data[mem->size - 1] = '\0';
        fclose(file);
        return bgfx::createShader(mem);
    }

    bgfx::ProgramHandle& Renderer::getProgramHandle()
    {
        return program;
    }

    bgfx::VertexLayout& Renderer::getVertexLayout()
    {
        return vLayout;
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
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, windowSize.width, windowSize.height);

        bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
        printf("shader handle %i created for vs_cubes.bin\n", vsh.idx);
        if (vsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return;
        }

        bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
        printf("shader handle %i created for fs_cubes.bin \n", fsh.idx);
        if (fsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return;
        }

        program = bgfx::createProgram(vsh, fsh, true);
        printf("program handle %i created\n", program.idx);

        vLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
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