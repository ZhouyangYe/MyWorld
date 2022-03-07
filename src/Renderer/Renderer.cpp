#include "Renderer.h"
#include "Model/Texture.h"

namespace MyWorld
{
    bgfx::ProgramHandle Renderer::texture_program = BGFX_INVALID_HANDLE;
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

        // set view clear and rect for default view
        bgfx::setViewClear(Tools::DEFAULT_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
        bgfx::setViewRect(Tools::DEFAULT_VIEW_ID, 0, 0, windowSize.width, windowSize.height);

        // set view clear and rect for water view
        bgfx::setViewClear(Tools::WATER_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
        bgfx::setViewRect(Tools::WATER_VIEW_ID, 0, 0, windowSize.width, windowSize.height);

        texture_program = Tools::createProgram("vs_texture", "fs_texture");
        texture_color_program = Tools::createProgram("vs_texture_color", "fs_texture_color");
        texture_array_program = Tools::createProgram("vs_texture_array", "fs_texture_array");
        texture_array_color_program = Tools::createProgram("vs_texture_array_color", "fs_texture_array_color");

        PosColorVertex::Init();
        PosTextureVertex::Init();
        PosTextureArrayVertex::Init();
        PosColorTextureVertex::Init();
        PosColorTextureArrayVertex::Init();

        Texture::Init({ param.windowSize.width, param.windowSize.height });
    }

    void Renderer::Terminate()
    {
        Texture::Destroy();
        Tools::Terminate();

        bgfx::destroy(texture_program);
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

    void Renderer::screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width, float _height)
    {
        if (3 == bgfx::getAvailTransientVertexBuffer(3, PosTextureVertex::layout))
        {
            bgfx::TransientVertexBuffer vb;
            bgfx::allocTransientVertexBuffer(&vb, 3, PosTextureVertex::layout);
            PosTextureVertex* vertex = (PosTextureVertex*)vb.data;

            const float minx = -_width;
            const float maxx = _width;
            const float miny = 0.0f;
            const float maxy = _height * 2.0f;

            const float texelHalfW = _texelHalf / _textureWidth;
            const float texelHalfH = _texelHalf / _textureHeight;
            const float minu = -1.0f + texelHalfW;
            const float maxu = 1.0f + texelHalfH;

            const float zz = 0.0f;

            float minv = texelHalfH;
            float maxv = 2.0f + texelHalfH;

            if (_originBottomLeft)
            {
                float temp = minv;
                minv = maxv;
                maxv = temp;

                minv -= 1.0f;
                maxv -= 1.0f;
            }

            vertex[0].x = minx;
            vertex[0].y = miny;
            vertex[0].z = zz;
            vertex[0].u = minu;
            vertex[0].v = minv;

            vertex[1].x = maxx;
            vertex[1].y = miny;
            vertex[1].z = zz;
            vertex[1].u = maxu;
            vertex[1].v = minv;

            vertex[2].x = maxx;
            vertex[2].y = maxy;
            vertex[2].z = zz;
            vertex[2].u = maxu;
            vertex[2].v = maxv;

            bgfx::setVertexBuffer(0, &vb);
        }
    }
}