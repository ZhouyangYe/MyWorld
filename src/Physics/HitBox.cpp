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

	// detect collision
	const bool&& HitBox::collide(HitBox& box)
	{
		AABB aabb_s = getAABB();
		AABB aabb_t = box.getAABB();
		return
			aabb_t.max.x - aabb_s.min.x >= -FLOAT_PRECISION &&
			aabb_t.max.y - aabb_s.min.y >= -FLOAT_PRECISION &&
			aabb_t.max.z - aabb_s.min.z >= -FLOAT_PRECISION &&
			aabb_s.max.x - aabb_t.min.x >= -FLOAT_PRECISION &&
			aabb_s.max.y - aabb_t.min.y >= -FLOAT_PRECISION &&
			aabb_s.max.z - aabb_t.min.z >= -FLOAT_PRECISION;
	}

	// get offsets for hitbox when there is a collision(collide with static object)
	const glm::vec3&& HitBox::getCollisionOffset(HitBox& box)
	{
		AABB aabb_s = getAABB();
		AABB aabb_t = box.getAABB();
		if (
			aabb_t.max.x - aabb_s.min.x > FLOAT_PRECISION &&
			aabb_t.max.y - aabb_s.min.y > FLOAT_PRECISION &&
			aabb_t.max.z - aabb_s.min.z > FLOAT_PRECISION &&
			aabb_s.max.x - aabb_t.min.x > FLOAT_PRECISION &&
			aabb_s.max.y - aabb_t.min.y > FLOAT_PRECISION &&
			aabb_s.max.z - aabb_t.min.z > FLOAT_PRECISION
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

	// get offsets for hitbox when there is a collision(collide with non-static object)
	const HitBox::Offset&& HitBox::getCollisionOffsets(HitBox& box)
	{
		glm::vec3 offset = getCollisionOffset(box);

		if (isStatic) return { glm::vec3{ 0.0f, 0.0f, 0.0f }, offset };
		if (box.isStatic) return { offset, glm::vec3{ 0.0f, 0.0f, 0.0f } };

		const float total_weight = weight + box.weight;
		return {
			offset * box.weight / total_weight,
			-offset * weight / total_weight
		};
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

	void HitBox::setVelocity(float&& x, float&& y, float&& z)
	{
		velocity.x = x;
		velocity.y = y;
		velocity.z = z;
	}

	void HitBox::setVelocityX(float&& v)
	{
		velocity.x = v;
	}

	void HitBox::setVelocityY(float&& v)
	{
		velocity.y = v;
	}

	void HitBox::setVelocityZ(float&& v)
	{
		velocity.z = v;
	}

	void HitBox::updatePos()
	{
		if (velocity.x == 0.0f && velocity.y == 0.0f && velocity.z == 0.0f) return;
		pos += velocity * (float)Time::getDeltaTime();
	}

	const glm::vec3& HitBox::getVelocity()
	{
		return velocity;
	}

	void HitBox::Register()
	{
		cubeVertices = Model::getVerticesType0(0xffffffff);
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 8 * sizeof(Renderer::PosColorVertex)), Renderer::PosColorVertex::layout);
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
