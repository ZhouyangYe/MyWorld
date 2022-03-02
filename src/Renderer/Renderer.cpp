#include "Renderer.h"
#include "Model/Texture.h"

namespace MyWorld
{
    bgfx::ProgramHandle Renderer::texture_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_color_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_array_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_array_color_program = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout Renderer::colorLayout;
    bgfx::VertexLayout Renderer::textureLayout;
    bgfx::VertexLayout Renderer::textureArrayLayout;
    bgfx::VertexLayout Renderer::colorTextureLayout;
    bgfx::VertexLayout Renderer::colorTextureArrayLayout;
    uint32_t Renderer::debugMode = BGFX_DEBUG_NONE;

    const bgfx::VertexLayout& Renderer::getColorLayout()
    {
        return colorLayout;
    }

    const bgfx::VertexLayout& Renderer::getTextureLayout()
    {
        return textureLayout;
    }

    const bgfx::VertexLayout& Renderer::getTextureArrayLayout()
    {
        return textureArrayLayout;
    }

    const bgfx::VertexLayout& Renderer::getColorTextureLayout()
    {
        return colorTextureLayout;
    }

    const bgfx::VertexLayout& Renderer::getColorTextureArrayLayout()
    {
        return colorTextureArrayLayout;
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

        texture_program = Tools::createProgram("vs_texture", "fs_texture");
        texture_color_program = Tools::createProgram("vs_texture_color", "fs_texture_color");
        texture_array_program = Tools::createProgram("vs_texture_array", "fs_texture_array");
        texture_array_color_program = Tools::createProgram("vs_texture_array_color", "fs_texture_array_color");

        colorLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
        textureLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
        textureArrayLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .end();
        colorTextureLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
        colorTextureArrayLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .end();

        Texture::Init();
    }

    void Renderer::Terminate()
    {
        Texture::Destroy();
        Tools::Terminate();

        bgfx::destroy(texture_program);
        bgfx::destroy(texture_color_program);
        bgfx::destroy(texture_array_program);
        bgfx::shutdown();
    }

    void Renderer::Begin()
    {

    }

    void Renderer::End()
    {
        bgfx::frame();
    }

    void Renderer::switchRenderMode()
    {
        if (debugMode & BGFX_DEBUG_WIREFRAME)
        {
            debugMode &= ~BGFX_DEBUG_WIREFRAME;
            bgfx::setDebug(debugMode);
        }
        else
        {
            // debug using wireframes
            debugMode |= BGFX_DEBUG_WIREFRAME;
            bgfx::setDebug(debugMode);
        }
    }

    bool Renderer::toggleDebugText()
    {
        if (debugMode & BGFX_DEBUG_TEXT)
        {
            debugMode &= ~BGFX_DEBUG_TEXT;
            bgfx::setDebug(debugMode);
            return false;
        }
        else
        {
            // show 2d debug text
            debugMode |= BGFX_DEBUG_TEXT;
            bgfx::setDebug(debugMode);
            return true;
        }
    }

    const uint32_t& Renderer::getDebugMode()
    {
        return debugMode;
    }
}