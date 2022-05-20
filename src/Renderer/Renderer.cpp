#include "Renderer.h"
#include "Model/Texture.h"

namespace MyWorld
{
    bgfx::ProgramHandle Renderer::color_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::water_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::outline_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_color_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_array_program = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle Renderer::texture_array_color_program = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout Renderer::PosColorVertex::layout;
    bgfx::VertexLayout Renderer::PosTextureVertex::layout;
    bgfx::VertexLayout Renderer::PosTextureArrayVertex::layout;
    bgfx::VertexLayout Renderer::PosColorTextureVertex::layout;
    bgfx::VertexLayout Renderer::PosColorTextureArrayVertex::layout;
    uint32_t Renderer::debugMode = BGFX_DEBUG_NONE;

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

        Tools::Init();

        // set view clear and rect for default view
        bgfx::setViewClear(Tools::DEFAULT_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
        bgfx::setViewRect(Tools::DEFAULT_VIEW_ID, 0, 0, windowSize.width, windowSize.height);


        color_program = Tools::createProgram("vs_color", "fs_color");
        water_program = Tools::createProgram("vs_water", "fs_water");
        texture_program = Tools::createProgram("vs_texture", "fs_texture");
        outline_program = Tools::createProgram("vs_outline", "fs_outline");
        texture_color_program = Tools::createProgram("vs_texture_color", "fs_texture_color");
        texture_array_program = Tools::createProgram("vs_texture_array", "fs_texture_array");
        texture_array_color_program = Tools::createProgram("vs_texture_array_color", "fs_texture_array_color");

        PosColorVertex::Init();
        PosTextureVertex::Init();
        PosTextureArrayVertex::Init();
        PosColorTextureVertex::Init();
        PosColorTextureArrayVertex::Init();
        Texture::Init();
    }

    void Renderer::Terminate()
    {
        Texture::Destroy();
        Tools::Terminate();

        bgfx::destroy(color_program);
        bgfx::destroy(water_program);
        bgfx::destroy(texture_program);
        bgfx::destroy(outline_program);
        bgfx::destroy(texture_color_program);
        bgfx::destroy(texture_array_program);
        bgfx::destroy(texture_array_color_program);
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