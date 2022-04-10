#pragma once
#include <list>
#include <vector> 
#include <time.h>
#include "PGP_EntityPrimitive.h"
#include "PGP_Renderer.h"
#include <random>

struct Peak {
	glm::vec2 position;
	int height;
};

class PGP_Generator {
public:
	static void InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList);
	static void DrawAllCubes(std::vector<std::list<Cube*>> &cubeList);
	static void CreateTerrain(std::vector<std::list<Cube*>> &cubeList);

private:
	static void CreateCubeAndPushToList(std::vector<std::list<Cube*>> &cubeList, ECubeType cubeType, glm::vec3 pos, float scale = 1.f);
	static int GetRandomNumber(int min, int max);
	static int GetDistance(glm::vec2 aPos, glm::vec2 bPos);
	static Peak* GetClosestPeak(std::vector<Peak*> &peaks, glm::vec2 pos);
	static bool srandInit;
};