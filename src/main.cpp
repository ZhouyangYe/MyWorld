#include <stdio.h>
#include <string.h>
#include <iostream>
#include <limits.h>
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "GLFW/glfw3.h"
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD 
    #define GLFW_EXPOSE_NATIVE_X11
#else
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include "GLFW/glfw3native.h"
#define WNDW_WIDTH 960
#define WNDW_HEIGHT 540
struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};
static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff888888 },
    { 1.0f,  1.0f,  1.0f, 0xff8888ff },
    {-1.0f, -1.0f,  1.0f, 0xff88ff88 },
    { 1.0f, -1.0f,  1.0f, 0xff88ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff8888 },
    { 1.0f,  1.0f, -1.0f, 0xffff88ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff88 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};
static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};
bgfx::ShaderHandle loadShader(const char *FILENAME)
{
    const char* shaderPath = "???";
    //dx11/  dx9/   essl/  glsl/  metal/ pssl/  spirv/
    bgfx::ShaderHandle invalid = BGFX_INVALID_HANDLE;
    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:     shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:    shaderPath = "c:\\Bright\\dev\\MyWorld\\vendors\\bgfx.cmake\\bgfx\\examples\\runtime\\shaders\\dx11\\";  break;
        case bgfx::RendererType::Gnm:           shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/pssl/";  break;
        case bgfx::RendererType::Metal:         shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/metal/"; break;
        case bgfx::RendererType::OpenGL:        shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:      shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:        shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/spirv/"; break;
        case bgfx::RendererType::Nvn:
        case bgfx::RendererType::WebGPU:
        case bgfx::RendererType::Count:         return invalid; // count included to keep compiler warnings happy
    }

    std::cout << shaderPath << std::endl;
    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    char *filePath = (char *)malloc(shaderLen + fileLen + 1);
    memcpy(filePath, shaderPath, shaderLen);
    memcpy(&filePath[shaderLen], FILENAME, fileLen);
    filePath[shaderLen+fileLen] = 0;    // properly null terminate
    std::cout << filePath << std::endl;
    FILE *file = fopen(filePath, "rb");
    
    if (!file) {
        return invalid;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);
    return bgfx::createShader(mem);
}
int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "Hello, bgfx!", NULL, NULL);
    bgfx::PlatformData pd;
    
    #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD 
    
        #if ENTRY_CONFIG_USE_WAYLAND // examples entry options define
            pd.ndt      = glfwGetWaylandDisplay(); 
        #else 
            pd.ndt      = glfwGetX11Display(); 
            pd.nwh = (void*)glfwGetX11Window(window);
        #endif 
        
    #elif BX_PLATFORM_OSX
     
            pd.ndt      = NULL; 
    
    #elif BX_PLATFORM_WINDOWS 
    
            pd.ndt      = NULL; 
            pd.nwh = glfwGetWin32Window(window);
    
    #endif // BX_PLATFORM_*
    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::Count; // pick one!
    
    // seems to default to vulkan which is fine by me!
    //bgfxInit.type = bgfx::RendererType::Vulkan;
    //bgfxInit.type = bgfx::RendererType::OpenGL;
    
    bgfxInit.resolution.width = WNDW_WIDTH;
    bgfxInit.resolution.height = WNDW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    
    // seems bgfx is bright enough to not use the active but unused integrated device!
    //bgfxInit.vendorId = BGFX_PCI_ID_NVIDIA; // just in case its selecting unused integrated device?
    
    bgfxInit.platformData = pd;
    bgfx::init(bgfxInit);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, WNDW_WIDTH, WNDW_HEIGHT);
    bgfx::VertexLayout pcvDecl;
    
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
    bgfx::ShaderHandle vsh = loadShader("vs_deferred_light.bin");
    printf("shader handle %i created for vs_cubes.bin\n", vsh.idx);
    if (vsh.idx == USHRT_MAX)
    {
        printf("*** shader model not supported or file not found ****\n");
        bgfx::shutdown();
        return -1;
    }
    
    bgfx::ShaderHandle fsh = loadShader("fs_deferred_light.bin");
    printf("shader handle %i created for fs_cubes.bin \n", fsh.idx);
    if (fsh.idx == USHRT_MAX)
    {
        printf("*** shader model not supported or file not found ****\n");
        bgfx::shutdown();
        return -1;
    }
    
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);
    printf("program handle %i created\n", program.idx);
    unsigned int counter = 0;
    while(!glfwWindowShouldClose(window)) 
    {
        const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(WNDW_WIDTH) / float(WNDW_HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        float mtx[16];
        bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.0081f);
        bgfx::setTransform(mtx);
        
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
    
        bgfx::submit(0, program);
        bgfx::frame();
        counter++;
        
        glfwPollEvents();
        if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
    bgfx::destroy(program);
    
    bgfx::destroy(ibh);
    bgfx::destroy(vbh);
    bgfx::shutdown();       
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
