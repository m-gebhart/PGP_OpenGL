#pragma once
#include <list>
#include <vector> 
#include <time.h>
#include "PGP_EntityPrimitive.h"
#include "PGP_Renderer.h"
#include <random>
#include <map>
#include <tuple>


class PGP_Generator {

public:
	static int noiseImgSize;
	static int terrainSize;
	static std::map<std::tuple<int, int, int>, ECubeType> cubeDict;

private:
	static bool srandInit;
	static unsigned char* noiseImgData;

	const static int waterLevel = 4;

public:
	static void InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList);
	static void DrawAllCubes(std::vector<std::list<Cube*>> &cubeList);
	static void CreateTerrain(std::vector<std::list<Cube*>> &cubeList);

private:
	static Cube* CreateCubeAndPushToList(std::vector<std::list<Cube*>> &cubeList, ECubeType cubeType, glm::vec3 pos, float scale = 1.f);
	static int GetRandomNumber(int min, int max);
	static int GetInterpHeightFromNoise(int xPos, int zPos, int minHeight, int maxHeight, int noiseStep);

	static bool SurroundedByWater(glm::vec3 pos);
};