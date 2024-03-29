#include "PGP_Generator.h"

/*struct NoiseImg*/
int NoiseImg::noiseImgSize = 16;
unsigned char* NoiseImg::noiseImgData = nullptr;
int NoiseImg::noiseSensitivity = 1;

/*class PGP_Generator*/
NoiseImg* PGP_Generator::noiseImg = nullptr;
int PGP_Generator::terrainSize = 1;
int PGP_Generator::terrainHeight;
int PGP_Generator::waterLevel;

void PGP_Generator::InitializeAllCubesList(std::vector<std::list<Cube*>> &emptyList)
{
	for (int i = 0; i < ECubeTypeSize; i++) 
	{
		static std::list<Cube*> newList = std::list<Cube*>();
		emptyList[i] = newList;
	}
}

std::unordered_map<std::pair<int, int>, std::pair<int, ECubeType>, custom_hash> PGP_Generator::PerlinDict2D;
std::unordered_map<std::tuple<int, int, int>, Cube*, custom_hash> PGP_Generator::CubeDict;

Cube* PGP_Generator::CreateCubeAndPushToList(std::vector<std::list<Cube*>>& cubeList, ECubeType cubeType, glm::vec3 pos, float scale, bool writeTo2DDict)
{
	Cube* newCube = PGP_EPrimitive::CreateCube(cubeType, pos, scale);
	cubeList[cubeType].push_back(newCube);
	if (writeTo2DDict)
		PerlinDict2D[std::make_pair(pos.x, pos.z)] = std::make_pair(pos.y, cubeType);
	CubeDict[std::make_tuple(pos.x, pos.y, pos.z)] = newCube;
	return newCube;
}

unsigned char* PGP_Generator::GenerateNoiseImgData(NoiseImg* outputImgData)
{
	int imageBPP;
	std::vector<unsigned char*> allTextureData;

	const int pngCount = 3; //reading three random png noise files
	for (int i = 0; i < pngCount; i++)
	{
		int randomPNGnr = GetRandomNumber(1, 5);
		std::string filePath = ".\\Ressources\\Noise\\noise_" + std::to_string(randomPNGnr) + ".png";
		allTextureData.push_back(PGP_Texture::LoadStaticTextureData(filePath.c_str(), noiseImg->noiseImgSize, noiseImg->noiseImgSize, imageBPP));
	}

	//finding average of these data
	unsigned char* avgTextureData = *allTextureData.begin();
	for (int i = 0; i < allTextureData.size(); i++)
	{
		for (int i = 0; i < pngCount; i++)
		{
			avgTextureData[i] = (allTextureData[0][i] + allTextureData[1][i] + allTextureData[2][i]) / 3;
		}
	}

	return avgTextureData;
}

void PGP_Generator::CreateTerrain(std::vector<std::list<Cube*>> &cubeList)
{
	InitializeAllCubesList(cubeList);
	noiseImg->noiseImgData = GenerateNoiseImgData(noiseImg);

	int noiseOffset = GetRandomNumber(0, noiseImg->noiseImgSize - terrainSize);
	terrainHeight = GetRandomNumber(10, 35);
	waterLevel = floor(terrainHeight * 0.4f);
	noiseImg->noiseSensitivity = GetRandomNumber(4, 32);

	Cube* prevCube = nullptr;
	Cube* cube = nullptr;

	for (int x = noiseOffset; x < noiseOffset + terrainSize; x++)
	{
		for (int z = noiseOffset; z < noiseOffset + terrainSize; z++)
		{
			glm::vec3 position = glm::vec3(x-noiseOffset, GetInterpHeightFromNoise(x, z, terrainGround, terrainHeight, noiseImg->noiseSensitivity), z-noiseOffset);
			ECubeType cubeType = ECubeType::ground;
			bool bWriteTo2DDict = true;
			
			//snow if too high
			if (PGP_Config::bGenerateSnow && position.y > terrainHeight*0.6f)
				cubeType = ECubeType::snow;

			// sand if water nearby
			if (position.y <= waterLevel)
			{
				if(bCloseToWater(position))
					cubeType = ECubeType::sand;
				//create lake if below water level
				if (position.y < waterLevel) 
				{
					for (int posY = position.y; posY < waterLevel; posY++)
						PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::water, glm::vec3(position.x, posY, position.z), 1.0f, false);
					PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::water, glm::vec3(position.x, waterLevel, position.z));
					bWriteTo2DDict = false;
				}
			}

			cube = PGP_Generator::CreateCubeAndPushToList(cubeList, cubeType, position, 1.0f, bWriteTo2DDict);
			int randomRot = GetRandomNumber(0, 3);
			if (randomRot > 0)
				PGP_EPrimitiveTransform::RotateCube(cube, randomRot * 90.f, glm::vec3(0, 1.f, 0));

			//add bushes randomly
			if (cubeType == ECubeType::ground)
			{
				int bushChance = GetRandomNumber(0, 100);
				if (bushChance > 85) {
					Cube* newBush = PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::bush, position + glm::vec3(0, 0.75f, 0), 0.25f, false);
					if (newBush)
						PGP_EPrimitiveTransform::RotateCube(newBush, 45, glm::vec3(0, 1.f, 0));
				}
			}

			//fill gaps with dirt
			if (prevCube)
			{
				//z axis
				if (cube->pivotPointPosition.y != prevCube->pivotPointPosition.y) 
				{
					glm::vec3 higherCubePos = cube->pivotPointPosition.y > prevCube->pivotPointPosition.y ? cube->pivotPointPosition : prevCube->pivotPointPosition;
					int cubeHeight = higherCubePos.y;
					int minHeight = std::min(cube->pivotPointPosition.y, prevCube->pivotPointPosition.y);
					while (cubeHeight - minHeight > waterLevel)
					{
						PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::dirt, glm::vec3(higherCubePos.x, --cubeHeight, higherCubePos.z), 1.0f, false);
					}
				}

				//x axis
				if (cube->pivotPointPosition.y != PerlinDict2D[std::make_pair(x-1, z)].first)
				{
					glm::vec3 higherCubePos = cube->pivotPointPosition.y > PerlinDict2D[std::make_pair(x - 1, z)].first ? cube->pivotPointPosition : glm::vec3(x-1, PerlinDict2D[std::make_pair(x - 1, z)].first, z);
					int cubeHeight = higherCubePos.y;
					int minHeight = std::min((int)cube->pivotPointPosition.y, PerlinDict2D[std::make_pair(x - 1, z)].first);
					while (cubeHeight - minHeight > waterLevel)
					{
						PGP_Generator::CreateCubeAndPushToList(cubeList, ECubeType::dirt, glm::vec3(higherCubePos.x, --cubeHeight, higherCubePos.z), 1.0f, false);
					}
				}
			}

			prevCube = cube;
		}
	}
}

int PGP_Generator::GetInterpHeightFromNoise(int xPos, int zPos, int minHeight, int maxHeight, int noiseStep)
{
	int imgPosition = ((xPos * 4 + zPos * noiseImg->noiseImgSize * 4));
	int noiseValue = int(noiseImg->noiseImgData[imgPosition % (int)pow(noiseImg->noiseImgSize, 2)]);

	return glm::mix(minHeight, maxHeight, noiseValue / 255.f);
}

int PGP_Generator::GetRandomNumber(int min, int max)
{
	unsigned rand_seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rand_generator(rand_seed);
	std::uniform_int_distribution<int> rand_distribution(min, max);
	return rand_distribution(rand_generator);
}

bool PGP_Generator::bCloseToWater(glm::vec3 pos)
{
	if (PerlinDict2D[std::make_pair((int)pos.x - 1, (int)pos.z)].second == ECubeType::water
		|| PerlinDict2D[std::make_pair((int)pos.x, (int)pos.z-1)].second == ECubeType::water
		|| GetInterpHeightFromNoise(pos.x + 1, pos.z, terrainGround, terrainHeight, noiseImg->noiseSensitivity) <= waterLevel+1
		|| GetInterpHeightFromNoise(pos.x, pos.z+1, terrainGround, terrainHeight, noiseImg->noiseSensitivity) <= waterLevel+1)
				return true;

	return false;
}

void PGP_Generator::ClearTerrain(std::vector<std::list<Cube*>>& cubeList)
{
	for (std::list<Cube*> cubeTypeList : cubeList)
		for (Cube* cube : cubeTypeList) 
			delete(cube);

	cubeList.clear();
	cubeList.shrink_to_fit();
	PerlinDict2D.clear();
	CubeDict.clear();
}