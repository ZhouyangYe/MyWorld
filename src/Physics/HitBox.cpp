#include "HitBox.h"

namespace MyWorld
{
	Renderer::PosColorVertex* HitBox::cubeVertices = nullptr;
	bgfx::VertexBufferHandle HitBox::vbh = BGFX_INVALID_HANDLE;
	const uint8_t HitBox::faces = 0;
	const uint64_t HitBox::state = Model::default_state | BGFX_STATE_PT_LINESTRIP;
	HitBox HitBox::terrainHitBox = HitBox(true, 0.0f, glm::vec3{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f);

	HitBox::~HitBox()
	{
		// Util::log("in");
	}

	const glm::vec3 HitBox::getCollisionOffset(HitBox& box)
	{
		if (
			aabb.min.x < box.aabb.max.x &&
			aabb.min.y < box.aabb.max.y &&
			aabb.min.z < box.aabb.max.z &&
			aabb.max.x > box.aabb.min.x &&
			aabb.max.y > box.aabb.min.y &&
			aabb.max.z > box.aabb.min.z
		)
		{
			float
				x1 = aabb.max.x - box.aabb.min.x,
				x2 = box.aabb.max.x - aabb.min.x,
				y1 = aabb.max.y - box.aabb.min.y,
				y2 = box.aabb.max.y - aabb.min.y,
				z1 = aabb.max.z - box.aabb.min.z,
				z2 = box.aabb.max.z - aabb.min.z;
			return glm::vec3{
				x1 < x2 ? -x1 : x2,
				y1 < y2 ? -y1 : y2,
				z1 < z2 ? -z1 : z2
			};
		}

		return glm::vec3{ 0.0f, 0.0f, 0.0f };
	}

	const HitBox::AABB& HitBox::getAABB()
	{
		if (aabb.min.x != pos.x || aabb.min.y != pos.y || aabb.min.z != pos.z)
		{
			aabb.Update(pos, pos + glm::vec3{ width, width, height });
		}
		return aabb;
	}

	const glm::vec3& HitBox::getCoords()
	{
		return pos;
	}

	void HitBox::setPos(glm::vec3& pos)
	{
		this->pos = pos;
	}

	void HitBox::setPos(glm::vec3&& pos)
	{
		this->pos = pos;
	}

	void HitBox::Register()
	{
		cubeVertices = Model::getVerticesType0(0xffffffff);
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 16 * sizeof(Renderer::PosColorVertex)), Renderer::PosColorVertex::layout);
	}

	void HitBox::Destroy()
	{
		if (bgfx::isValid(vbh)) bgfx::destroy(vbh);
		if (cubeVertices != nullptr)
		{
			delete[] cubeVertices;
			cubeVertices = nullptr;
		}
	}

	void HitBox::Draw()
	{
		Model::DrawStatic(Tools::DEFAULT_VIEW_ID, vbh, Model::getIbh(faces), Renderer::color_program, state, pos, scale);
	}
}
