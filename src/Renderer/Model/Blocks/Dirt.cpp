#include "Dirt.h"

namespace MyWorld
{
	Block::PosTextureArrayVertex* Dirt::cubeVertices_ta = nullptr;
	Block::PosTextureVertex* Dirt::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Dirt::vbh = BGFX_INVALID_HANDLE;

	void Dirt::Register()
	{
		const glm::vec2 face{ 3.0f, 1.0f };

		if (Texture::isArrayBufferSupported())
		{
			cubeVertices_ta = Block::getVerticesType3(face, face, face);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices_ta, 16 * sizeof(Block::PosTextureArrayVertex)), Renderer::getTextureArrayLayout());
		}
		else
		{
			cubeVertices = Block::getVerticesType1(face, face, face);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
		}
	}

	void Dirt::Destroy()
	{
		bgfx::destroy(vbh);
		delete[] cubeVertices;
		cubeVertices = nullptr;
		delete[] cubeVertices_ta;
		cubeVertices_ta = nullptr;
	}

	Dirt::Dirt()
	{}

	Dirt::Dirt(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::DIRT, coords, chunk_coords), program(Texture::isArrayBufferSupported() ? Renderer::texture_array_program : Renderer::texture_program)
	{
	}

	Dirt::~Dirt()
	{}

	void Dirt::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program);
	}
}
