#pragma once
#include <list>
#include <vector> 
#include <time.h>
#include "PGP_EntityPrimitive.h"
#include <random>
#include <unordered_map>
#include <tuple>
#include <utility>


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

struct custom_hash //neded to use pairs and tuples as keys in unordered_map
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}

	template <class T1, class T2, class T3>
	std::size_t operator() (const std::tuple<T1, T2, T3>& tuple) const {
		return std::hash<T1>()(std::get<0>(tuple)) ^ std::hash<T2>()(std::get<1>(tuple)) ^ std::hash<T3>()(std::get<2>(tuple));
	}
};

class PGP_Generator {

public:
	static int terrainSize;
	static std::unordered_map<std::pair<int, int>, std::pair<int, ECubeType>, custom_hash> PerlinDict2D; //save height and type for each perlin-generated cube on xz-plane 
	static std::unordered_map<std::tuple<int, int, int>, Cube*, custom_hash> CubeDict; //cube of xyz-pos 


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