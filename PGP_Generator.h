#pragma once
#include <list>
#include <vector> 
#include <time.h>
#include "PGP_EntityPrimitive.h"
#include <random>
#include <map>
#include <tuple>


struct NoiseImg {
	static int noiseImgSize;
	static unsigned char* noiseImgData;
	static int noiseSensitivity;

	void SetImgData(unsigned char* newImgData)
	{
		noiseImgData = newImgData;
	}

	void SetImgSize(int newImgSize)
	{
		noiseImgSize = newImgSize;
	}

	void SetNoiseSensitivty(int newImgSensitivy)
	{
		noiseSensitivity = newImgSensitivy;
	}
};

class PGP_Generator {

public:
	static int terrainSize;
	static std::map<std::pair<int, int>, std::pair<int, ECubeType>> PerlinDict2D; //save height and type for each perlin-generated cube on xz-plane 
	static std::map<std::tuple<int, int, int>, Cube*> CubeDict; //cube of xyz-pos 


private:
	static NoiseImg* noiseImg;
	static bool srandInit;
	static int srandSeed;

	static int terrainHeight;
	static int terrainGround;
	static int waterLevel;

public:
	static void InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList);
	static void CreateTerrain(std::vector<std::list<Cube*>> &cubeList);
	static void ClearTerrain(std::vector<std::list<Cube*>>& cubeList);

private:
	static unsigned char* GenerateNoiseImgData(NoiseImg* outputImg);

	static Cube* CreateCubeAndPushToList(std::vector<std::list<Cube*>> &cubeList, ECubeType cubeType, glm::vec3 pos, float scale = 1.f, bool writeTo2DDict = true);
	static int GetRandomNumber(int min, int max);
	static int GetInterpHeightFromNoise(int xPos, int zPos, int minHeight, int maxHeight, int noiseStep);

	static bool bCloseToWater(glm::vec3 pos);
};