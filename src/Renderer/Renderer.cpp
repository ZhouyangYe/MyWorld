#include "Renderer.h"

namespace MyWorld
{
    bgfx::ProgramHandle Renderer::program = {};
    bgfx::VertexLayout Renderer::colorLayout;
    bgfx::VertexLayout Renderer::textureLayout;
    bgfx::VertexLayout Renderer::colorTextureLayout;

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
        Tools::Init();

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

        program = Tools::createProgram("vs_texture", "fs_texture");
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
        Tools::Terminate();

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