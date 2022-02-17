#include "Dirt.h"

namespace MyWorld
{
	Block::PosTextureVertex* Dirt::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Dirt::vbh;

	void Dirt::Register()
	{
		cubeVertices = Block::getVerticesType1(glm::vec2{ 3.0f, 1.0f }, glm::vec2{ 3.0f, 1.0f }, glm::vec2{ 3.0f, 1.0f });
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
	}

	void Dirt::Destroy()
	{
		bgfx::destroy(vbh);
		delete cubeVertices;
		cubeVertices = nullptr;
	}

	Dirt::Dirt()
	{}

	Dirt::Dirt(glm::vec3 coords) : Block(coords)
	{
	}

	Dirt::~Dirt()
	{}

	void Dirt::Draw(uint8_t& idx)
	{
		Block::Draw(vbh, Block::getIbh(idx));
	}
}
