#include "Grass.h"

namespace MyWorld
{
	Block::PosTextureArrayVertex* Grass::cubeVertices_ta = nullptr;
	Block::PosTextureVertex* Grass::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Grass::vbh = BGFX_INVALID_HANDLE;

	void Grass::Register()
	{
		const glm::vec2 side{ 2.0f, 1.0f };
		const glm::vec2 top{ 1.0f, 1.0f };
		const glm::vec2 bottom{ 3.0f, 1.0f };

		if (Texture::isArrayBufferSupported())
		{
			cubeVertices_ta = Block::getVerticesType3(side, top, bottom);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices_ta, 16 * sizeof(Block::PosTextureArrayVertex)), Renderer::getTextureArrayLayout());
		}
		else
		{
			cubeVertices = Block::getVerticesType1(side, top, bottom);
			vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
		}
	}

	void Grass::Destroy()
	{
		bgfx::destroy(vbh);
		delete[] cubeVertices;
		cubeVertices = nullptr;
		delete[] cubeVertices_ta;
		cubeVertices_ta = nullptr;
	}

	Grass::Grass()
	{}

	Grass::Grass(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::GRASS, coords, chunk_coords), program(Texture::isArrayBufferSupported() ? Renderer::texture_array_program : Renderer::texture_program)
	{
	}

	Grass::~Grass()
	{}

	void Grass::Draw(const uint8_t& faces)
	{
		Block::Draw(vbh, Block::getIbh(faces), program);
	}
}
