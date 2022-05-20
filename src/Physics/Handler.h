#pragma once
#include "HitBox.h"
#include "../Core/World/Chunk.h"

namespace MyWorld
{
	class Handler
	{
	private:
		HitBox* hitBox;
		glm::vec3 prevPos;
		bool&& notCollide(Block::TYPE type);
	public:
		void setPrevPos(glm::vec3& pos);
		void applyGravity();
		void commitVelocity();
		bool&& handleTerrainCollision(uint8_t& hitX, uint8_t& hitY, uint8_t& hitZ, Block::TYPE& hitType);
		Handler(HitBox* box) : hitBox(box), prevPos(box->getCoords()) {}
	};
}
