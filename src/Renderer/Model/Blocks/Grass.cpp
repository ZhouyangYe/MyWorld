#include "Grass.h"

namespace MyWorld
{
	Block::PosTextureVertex* Grass::cubeVertices = nullptr;
	bgfx::VertexBufferHandle Grass::vbh;

	void Grass::Register()
	{
		cubeVertices = Block::getVerticesType1(glm::vec2{ 2.0f, 1.0f }, glm::vec2{ 1.0f, 1.0f }, glm::vec2{ 3.0f, 1.0f });
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Block::PosTextureVertex)), Renderer::getTextureLayout());
	}

	void Grass::Destroy()
	{
		bgfx::destroy(vbh);
		delete cubeVertices;
		cubeVertices = nullptr;
	}

	Grass::Grass()
	{}

	Grass::Grass(glm::vec3 coords) : Block(coords)
	{
	}

	Grass::~Grass()
	{}

	void Grass::Draw()
	{
		Block::Draw(vbh, Block::ibh_t1);
	}
}
