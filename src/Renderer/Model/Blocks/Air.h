#pragma once
#include "Block.h"

namespace MyWorld
{
	class Air : public Block
	{
	public:
		Air();
		Air(glm::vec3 coords);
		~Air();
		void Draw(uint8_t& faces) override;
	};
}
