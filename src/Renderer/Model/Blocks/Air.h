#pragma once
#include "Block.h"

namespace MyWorld
{
	class Air : public Block
	{
	public:
		Air();
		Air(glm::vec3 coords, glm::vec2 chunk_coords);
		~Air();
	};
}
