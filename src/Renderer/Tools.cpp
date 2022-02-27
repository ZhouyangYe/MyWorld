#include "Tools.h"

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

    static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
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

    static void imageReleaseCb(void* _ptr, void* _userData)
    {
        BX_UNUSED(_ptr);
        bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
        bimg::imageFree(imageContainer);
    }

    void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
    {
        if (bx::open(_reader, _filePath))
        {
            uint32_t size = (uint32_t)bx::getSize(_reader);
            void* data = BX_ALLOC(_allocator, size);
            bx::read(_reader, data, size, bx::ErrorAssert{});
            bx::close(_reader);
            if (NULL != _size)
            {
                *_size = size;
            }
            return data;
        }
        else
        {
            DBG("Failed to open: %s.", _filePath);
        }

        if (NULL != _size)
        {
            *_size = 0;
        }

        return NULL;
    }

    void unload(void* _ptr)
    {
        BX_FREE(g_allocator, _ptr);
    }

    bx::FileReaderI* Tools::_reader;

    bgfx::ShaderHandle Tools::loadShader(const char* _name)
    {
        char filePath[512];

        const char* shaderPath = "???";

        //dx11/  dx9/   essl/  glsl/  metal/ pssl/  spirv/
        bgfx::ShaderHandle invalid = BGFX_INVALID_HANDLE;
        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D9:     shaderPath = "vendors/bgfx.cmake/bgfx/examples/runtime/shaders/dx9/";   break;
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:    shaderPath = "C:\\BrighT\\Dev\\MyWorld\\resource\\shaders\\bin\\";  break;
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

    bgfx::TextureHandle Tools::loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
    {
        BX_UNUSED(_skip);
        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

        uint32_t size;
        void* data = load(_reader, g_allocator, _filePath, &size);
        if (NULL != data)
        {
            bimg::ImageContainer* imageContainer = bimg::imageParse(g_allocator, data, size);

            if (NULL != imageContainer)
            {
                if (NULL != _orientation)
                {
                    *_orientation = imageContainer->m_orientation;
                }

                const bgfx::Memory* mem = bgfx::makeRef(
                    imageContainer->m_data
                    , imageContainer->m_size
                    , imageReleaseCb
                    , imageContainer
                );
                unload(data);

                if (imageContainer->m_cubeMap)
                {
                    handle = bgfx::createTextureCube(
                        uint16_t(imageContainer->m_width)
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , _flags
                        , mem
                    );
                }
                else if (1 < imageContainer->m_depth)
                {
                    handle = bgfx::createTexture3D(
                        uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , uint16_t(imageContainer->m_depth)
                        , 1 < imageContainer->m_numMips
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , _flags
                        , mem
                    );
                }
                else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags))
                {
                    handle = bgfx::createTexture2D(
                        uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , _flags
                        , mem
                    );
                }

                if (bgfx::isValid(handle))
                {
                    bgfx::setName(handle, _filePath);
                }

                if (NULL != _info)
                {
                    bgfx::calcTextureSize(
                        *_info
                        , uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , uint16_t(imageContainer->m_depth)
                        , imageContainer->m_cubeMap
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    );
                }
            }
        }

        return handle;
    }

    bgfx::TextureHandle Tools::loadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
    {
        return loadTexture(_reader, _name, _flags, _skip, _info, _orientation);
    }

    bgfx::ProgramHandle Tools::createProgram(const char* vs_name, const char* fs_name)
    {
        bgfx::ShaderHandle vsh = Tools::loadShader(vs_name);
        printf("shader handle %i created for vs_texture.bin\n", vsh.idx);
        if (vsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return BGFX_INVALID_HANDLE;
        }

        bgfx::ShaderHandle fsh = Tools::loadShader(fs_name);
        printf("shader handle %i created for fs_texture.bin \n", fsh.idx);
        if (fsh.idx == USHRT_MAX)
        {
            printf("*** shader model not supported or file not found ****\n");
            bgfx::shutdown();
            return BGFX_INVALID_HANDLE;
        }

        return bgfx::createProgram(vsh, fsh, true);
    }

    void Tools::Init()
    {
        _reader = BX_NEW(g_allocator, FileReader);

    }

    void Tools::Terminate()
    {
        unload(_reader);
    }
}
