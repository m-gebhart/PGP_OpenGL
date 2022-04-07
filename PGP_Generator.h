#pragma once
#include <list>
#include "PGP_EntityPrimitive.h"


class PGP_Generator {
public:
	static void InitializeAllCubesList(std::vector<std::list<Cube*>> emptyList);
	static void DrawAllCubes(std::vector<std::list<Cube*>> cubeList);
	//static void CreateTerrain(void);
};