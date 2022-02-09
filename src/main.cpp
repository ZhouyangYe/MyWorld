#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Window/Window.h"

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff }
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7
};

int main(void)
{
    MyWorld::WindowParams windowParams;
    MyWorld::Window::Init(windowParams);
    MyWorld::WindowSize windowSize = MyWorld::Window::getWindowSize();

    MyWorld::Renderer::Init({ MyWorld::Window::getWindowHWND(), { windowSize.width, windowSize.height } });
    MyWorld::Camera::Init({ { windowSize.width, windowSize.height } });

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), MyWorld::Renderer::getVertexLayout());
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    unsigned int counter = 0;

    while(!MyWorld::Window::shouldWindowTerminate())
    {
        MyWorld::Window::Begin();

        MyWorld::Camera::Begin();

        for (uint32_t yy = 0; yy < 11; ++yy)
        {
            for (uint32_t xx = 0; xx < 11; ++xx)
            {
                float mtx[16];
                bx::mtxRotateXY(mtx, counter * 0.01f +  xx * 0.21f, counter * 0.0081f + yy * 0.37f);
                mtx[12] = -15.0f + float(xx) * 3.0f;
                mtx[13] = -15.0f + float(yy) * 3.0f;
                mtx[14] = 0.0f;

                // Set model matrix for rendering.
                bgfx::setTransform(mtx);

                // Set vertex and index buffer.
                bgfx::setVertexBuffer(0, vbh);
                bgfx::setIndexBuffer(ibh);

                // Submit primitive for rendering to view 0.
                bgfx::submit(0, MyWorld::Renderer::getProgramHandle());
            }
        }

        bgfx::frame();
        counter++;

        MyWorld::Window::End();

        MyWorld::Camera::End();
    }

    MyWorld::Renderer::Terminate();
    bgfx::destroy(ibh);
    bgfx::destroy(vbh);  

    MyWorld::Camera::Terminate();
    
    MyWorld::Window::Terminate();
    return 0;
}
