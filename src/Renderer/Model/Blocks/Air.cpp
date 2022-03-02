#include "Air.h"

namespace MyWorld
{
	Air::Air()
	{}

	Air::Air(glm::vec3 coords) : Block(Block::AIR, coords)
	{}

	Air::~Air()
	{}

	void Air::Draw(const uint8_t& faces)
	{}
}