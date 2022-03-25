#pragma once
#include "HitBox.h"
#include "../Core/Chunk.h"

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
		bool&& handleTerrainCollision(uint8_t& hitX, uint8_t& hitY, uint8_t& hitZ);
		Handler(HitBox* box) : hitBox(box), prevPos(box->getCoords()) {}
	};
}
