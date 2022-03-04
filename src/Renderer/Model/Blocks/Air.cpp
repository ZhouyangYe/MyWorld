#include "Air.h"

namespace MyWorld
{
	Air::Air()
	{}

	Air::Air(glm::vec3 coords, glm::vec2 chunk_coords) : Block(Block::AIR, coords, chunk_coords)
	{}

	Air::~Air()
	{}

	void Air::Draw(const uint8_t& faces)
	{}
}
