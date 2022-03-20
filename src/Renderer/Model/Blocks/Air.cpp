#include "Air.h"

namespace MyWorld
{
	Air::Air()
	{}

	Air::Air(glm::vec3 coords, glm::vec2 chunk_coords, int chunk_id) : Block(Block::TYPE::AIR, coords, chunk_coords, chunk_id)
	{}

	Air::~Air()
	{}
}
