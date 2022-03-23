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
		AABB aabb_s = getAABB();
		AABB aabb_t = box.getAABB();
		if (
			aabb_t.max.x - aabb_s.min.x > float_precision &&
			aabb_t.max.y - aabb_s.min.y > float_precision &&
			aabb_t.max.z - aabb_s.min.z > float_precision &&
			aabb_s.max.x - aabb_t.min.x > float_precision &&
			aabb_s.max.y - aabb_t.min.y > float_precision &&
			aabb_s.max.z - aabb_t.min.z > float_precision
		)
		{
			float
				x1 = aabb_s.max.x - aabb_t.min.x,
				x2 = aabb_t.max.x - aabb_s.min.x,
				y1 = aabb_s.max.y - aabb_t.min.y,
				y2 = aabb_t.max.y - aabb_s.min.y,
				z1 = aabb_s.max.z - aabb_t.min.z,
				z2 = aabb_t.max.z - aabb_s.min.z;
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
