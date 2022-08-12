#pragma once
struct PGP_Config 
{
	static const int screenWidth = 1920;
	static const int screenHeight = 1280;

	static const int terrainSize = 50;
	static const bool bGenerateSnow = false;

	static constexpr glm::fvec3 skyBoxColor = glm::fvec3(135.f, 206.f, 235.f);

	static const int spawnAnimTime = 9;
	static const int clearAnimTime = 4;
};