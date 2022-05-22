#include "TerrainGeneration.h"

namespace MyWorld
{
	FastNoiseLite TerrainGeneration::noise;
	int TerrainGeneration::renderDistance = 20;
	// numbers of chunks to be rendered based on the player
	int TerrainGeneration::renderDistanceNum = renderDistance * CHUNK_WIDTH;
	int TerrainGeneration::chunk_num = renderDistance * renderDistance * 4;
	bool TerrainGeneration::showWorldBorder = true;

	void TerrainGeneration::setShowWorldBorder(bool&& show)
	{
		showWorldBorder = show;
	}

	void TerrainGeneration::setChunkRenderDistance(int& num)
	{
		renderDistance = num;
	}

	void TerrainGeneration::setChunkRenderDistanceNum(int& num)
	{
		renderDistanceNum = num;
	}

	void TerrainGeneration::setChunkNum(int& num)
	{
		renderDistanceNum = num;
	}

	const bool& TerrainGeneration::ShowWorldBorder()
	{
		return showWorldBorder;
	}

	const int& TerrainGeneration::ChunkRenderDistance()
	{
		return renderDistance;
	}

	const int& TerrainGeneration::ChunkRenderDistanceNum()
	{
		return renderDistanceNum;
	}

	const int& TerrainGeneration::ChunkNum()
	{
		return chunk_num;
	}

	// TODO: implement more interesting terrain
	const Block::TYPE&& TerrainGeneration::getType(glm::vec3& pos)
	{
		if (showWorldBorder)
		{
			int width = renderDistanceNum;
			if (
				pos.x > width - 1 ||
				pos.x < -width ||
				pos.y > width - 1 ||
				pos.y < -width ||
				pos.z > CHUNK_DEPTH - 1 ||
				pos.z < 0
				)
			{
				return Block::TYPE::AIR;
			}
		}

		const float data = (noise.GetNoise(pos.x, pos.y) + 1) / 2;
		const int surface = (int)(CHUNK_DEPTH * data);
		const int edge = surface - 1;
		const int waterLine = 190;

		if (pos.z < surface)
		{
			if (pos.z == edge && pos.z >= waterLine)
			{
				return Block::TYPE::GRASS;
			}
			else
			{
				return Block::TYPE::DIRT;
			}
		}
		else if (pos.z <= waterLine)
		{
			return Block::TYPE::WATER;
		}
		else
		{
			return Block::TYPE::AIR;
		}
	}

	void TerrainGeneration::Init()
	{
		// configure noise setting
		noise.SetSeed(666);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.003f);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFractalOctaves(6);
		noise.SetFractalLacunarity(1.5f);
		noise.SetFractalGain(1.4f);
		noise.SetFractalWeightedStrength(0.4f);
		// noise.SetFractalPingPongStrength(1.5f);
	}
}