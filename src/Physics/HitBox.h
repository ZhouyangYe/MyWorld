#pragma once
#include "../Core/Util.h"
#include "../Renderer/Model/Model.h"
#include "../Core/Time.h"

#define FLOAT_PRECISION 0.00001 // near 0 detection(solve float precision issue)
#define GRAVITY -0.6f           // vertical acceleration
#define MAX_SPEED 6.0f          // max speed

namespace MyWorld
{
	class HitBox
	{
	public:
		struct AABB
		{
			glm::vec3 min;
			glm::vec3 max;
			void Update(glm::vec3& min, glm::vec3& max)
			{
				this->min.x = min.x;
				this->min.y = min.y;
				this->min.z = min.z;
				this->max.x = max.x;
				this->max.y = max.y;
				this->max.z = max.z;
			}
			AABB(glm::vec3& min, glm::vec3& max) : min(min), max(max) {}
		};
		struct Offset
		{
			const glm::vec3 offset1;
			const glm::vec3 offset2;
		};
		static HitBox terrainHitBox;
		static void Register();
		static void Destroy();
	private:
		static Renderer::PosColorVertex* cubeVertices;
		static bgfx::VertexBufferHandle vbh;
		static const uint8_t faces;
		static const uint64_t state;
	public:
		const bool isStatic; // can or cannot move
		const float width;   // width
		const float height;  // height
		const AABB& getAABB();
		const glm::vec3& getCoords();
		const bool&& collide(HitBox& box);
		const glm::vec3&& getCollisionOffset(HitBox& box);
		const Offset&& getCollisionOffsets(HitBox& box);
		void setPos(glm::vec3& pos);
		void setPos(glm::vec3&& pos);
		void setVelocityX(float&& v);
		void setVelocityY(float&& v);
		void setVelocityZ(float&& v);
		void applyGravity();
		HitBox()
			: isStatic(true), weight(0.0f), pos(glm::vec3{ 0.0f, 0.0f, 0.0f }), width(1.0f), height(1.0f), aabb(pos, glm::vec3{ 1.0f, 1.0f, 1.0f }), scale(glm::vec3{ 1.0f, 1.0f, 1.0f }) {}
		HitBox(bool&& isStatic, float&& weight, glm::vec3&& pos, float&& width, float&& height)
			: isStatic(isStatic), weight(weight), pos(pos), width(width), height(height), aabb(pos, pos + glm::vec3{ width, width, height }), scale(glm::vec3{ width, width, height }) {}
		~HitBox();
		void Draw();
	private:
		float weight;       // mass
		glm::vec3 pos;      // coordinates
		glm::vec3 scale;    // scale
		glm::vec3 forward;  // forward
		glm::vec3 velocity; // velocity
		AABB aabb;          // aabb used to detect collision
	};
}
