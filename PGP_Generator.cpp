#include "PGP_Generator.h"
#include <iostream>

void PGP_Generator::InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList)
{
	for (int i = 0; i < ECubeTypeSize; i++) 
	{
		static std::list<Cube*> newList = std::list<Cube*>();
		emptyList[i] = newList;
	}
}

std::map<std::tuple<int, int, int>, ECubeType> PGP_Generator::cubeDict;

Cube* PGP_Generator::CreateCubeAndPushToList(std::vector<std::list<Cube*>>& cubeList, ECubeType cubeType, glm::vec3 pos, float scale)
{
	Cube* newCube = PGP_EPrimitive::CreateCube(cubeType, pos, scale);
	cubeList[cubeType].push_back(newCube);
	cubeDict[std::make_tuple((int)pos.x, (int)pos.y, (int)pos.z)] = cubeType;
	return newCube;
}

unsigned char* PGP_Generator::noiseImgData = nullptr;
int PGP_Generator::noiseImgSize = 0;
int PGP_Generator::terrainSize = 30;

void PGP_Generator::CreateTerrain(std::vector<std::list<Cube*>> &cubeList)
{
	int imageBPP;
	noiseImgData = PGP_Texture::LoadStaticTextureData(".\\Ressources\\Noise\\noise_1.png", noiseImgSize, noiseImgSize, imageBPP);

	int noiseOffset = GetRandomNumber(0, noiseImgSize - terrainSize);
	int terrainHeight = GetRandomNumber(25, 35);
	int waterLevel = terrainHeight * 0.35f;
	int noiseSensitivity = GetRandomNumber(4, 32);

	Cube* prevCube = nullptr;
	Cube* cube = nullptr;


	for (int x = noiseOffset; x < noiseOffset + terrainSize; x++)
	{
		for (int z = noiseOffset; z < noiseOffset + terrainSize; z++)
		{
			glm::vec3 position = glm::vec3(x-noiseOffset, GetInterpHeightFromNoise(x, z, 0, terrainHeight, 32), z-noiseOffset);
			ECubeType cubeType = ECubeType::ground;
			
			//snow if too high
			if (position.y > terrainHeight*0.6f)
				cubeType = ECubeType::snow;

			//sand if water nearby
			if (position.y == waterLevel && bSurroundedByWater(position))
				cubeType = ECubeType::sand;

			//create lake if valley
			else if (position.y < waterLevel) 
			{
				cubeType = ECubeType::sand;
				PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::water, position + glm::vec3(0, 1.f, 0));
			}
			cube = PGP_Generator::CreateCubeAndPushToList(cubeList, cubeType, position);

			//add bushes randomly
			if (cubeType == ECubeType::ground) 
			{
				int bushChance = GetRandomNumber(0, 100);
				if (bushChance > 85) {
					Cube* newBush = PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::bush, position + glm::vec3(0, 0.75f, 0), 0.25f);
					PGP_EPrimitiveTransform::RotateCube(newBush, 45, glm::vec3(0, 1.f, 0));
				}
			}

			//fill gaps with stones
			if (prevCube) 
			{
				Cube* higherCube = cube->pivotPointPosition.y > prevCube->pivotPointPosition.y ? cube : prevCube;
				int cubeHeight = higherCube->pivotPointPosition.y;
				while (cubeHeight - std::min(cube->pivotPointPosition.y, prevCube->pivotPointPosition.y) > 1)
				{
					PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::stone, glm::vec3(higherCube->pivotPointPosition.x, --cubeHeight, higherCube->pivotPointPosition.z));
				}
			}

			prevCube = cube;
		}
	}
}

int PGP_Generator::GetInterpHeightFromNoise(int xPos, int zPos, int minHeight, int maxHeight, int noiseStep)
{
	int imgPosition = ((xPos * 4 + zPos * noiseImgSize * 4));
	int noiseValue = int(noiseImgData[imgPosition % (int)pow(noiseImgSize, 2)]);

	return glm::mix(minHeight, maxHeight, noiseValue / 255.f);
}

bool PGP_Generator::srandInit = false;
int PGP_Generator::GetRandomNumber(int min, int max)
{
	if (!srandInit) {
		srand(time(NULL));
		srandInit = true;
	}
	return (rand() % (max - min)) + min;
}

bool PGP_Generator::bSurroundedByWater(glm::vec3 pos)
{
	for (int x = -1; x < 2; x++)
		for (int z = -1; z < 2; z++)
			if (cubeDict[std::make_tuple((int)pos.x+x, (int)pos.y, (int)pos.z+z)] == ECubeType::water)
				return true;

	return false;
}