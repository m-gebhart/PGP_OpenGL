#pragma once
#include <list>
#include <vector> 
#include <time.h>
#include "PGP_EntityPrimitive.h"
#include <random>
#include <map>
#include <tuple>


class PGP_Generator {

public:
	static int noiseImgSize;
	static int terrainSize;
	static std::map<std::pair<int, int>, std::pair<int, ECubeType>> CubeDict2D;

private:
	static bool srandInit;
	static unsigned char* noiseImgData;
	
	static int terrainHeight;
	static int terrainGround;
	static int waterLevel;
	static int noiseSensitivity;

public:
	static void InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList);
	static void CreateTerrain(std::vector<std::list<Cube*>> &cubeList);

private:
	static Cube* CreateCubeAndPushToList(std::vector<std::list<Cube*>> &cubeList, ECubeType cubeType, glm::vec3 pos, float scale = 1.f, bool writeToDict = true);
	static int GetRandomNumber(int min, int max);
	static int GetInterpHeightFromNoise(int xPos, int zPos, int minHeight, int maxHeight, int noiseStep);

	static bool bCloseToWater(glm::vec3 pos);
};