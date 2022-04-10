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

void PGP_Generator::DrawAllCubes(std::vector<std::list<Cube*>> &cubeList)
{
	for (unsigned int i = 0; i < cubeList.size(); i++)
		PGP_Renderer::UpdateAndDrawCubes(cubeList[i]);
}

void PGP_Generator::CreateCubeAndPushToList(std::vector<std::list<Cube*>>& cubeList, ECubeType cubeType, glm::vec3 pos, float scale)
{
	cubeList[cubeType].push_back(PGP_EPrimitive::CreateCube(cubeType, pos, scale));
}


void PGP_Generator::CreateTerrain(std::vector<std::list<Cube*>> &cubeList)
{
	/*--- WORK IN PROGRESS ---*/
	/*Degree of Pseudo-Randomness is not yet satisfying*/
	std::vector<Peak*> randPeaks;

	const int boardSize = GetRandomNumber(20, 30);
	int peakNr = GetRandomNumber(1, 4);
	int randHeight = GetRandomNumber(3, boardSize / 2);

	for (int peak = 0; peak < peakNr; peak++)
	{
		Peak* newPeak = new Peak();
		newPeak->position = glm::vec2(GetRandomNumber(3, boardSize), GetRandomNumber(3, boardSize));
		newPeak->height = randHeight - peak;
		randPeaks.push_back(newPeak);

		std::cout << randPeaks[peak]->position.x <<"; " << randPeaks[peak]->position.y << std::endl;
	}

	int randRadius = GetRandomNumber(0, 3);
	int randSnowCap = GetRandomNumber(randHeight-3, randHeight);
	int randFloorHeight = GetRandomNumber(0, 2);

	for (int x = 0; x < boardSize; x++)
	{
		for (int z = 0; z < boardSize; z++)
		{
			glm::vec3 position = glm::vec3(0.f);
			ECubeType cubeType = ground;

			//find closest peak
			Peak* closestPeak = GetClosestPeak(randPeaks, glm::vec2(x, z));
			glm::vec2 closestPeakPos = closestPeak->position;

			if (x == closestPeakPos.x && z == closestPeakPos.y || //peak
				((x >= closestPeakPos.x - randRadius && x <= closestPeakPos.x + randRadius //surrounding peak pos on x-axis: should be same height as peak
					&& z >= closestPeakPos.y - randRadius && z <= closestPeakPos.y + randRadius))) //surrounding peak pos on z-axis: should be same height as peak
			{
				position = glm::vec3(1.0f * x, closestPeak->height, 1.0f * z);
			}
			else //downhill y-Pos calculation
			{
				int distance = GetDistance(glm::vec2(x, z), closestPeakPos);
				position = glm::vec3(1.0f * x, closestPeak->height - distance + randRadius, 1.0f * z);
			}

			if (position.y > randSnowCap)
				cubeType = snow;
			else if (position.y <= randFloorHeight)
			{
				cubeType = position.y == randFloorHeight ? sand : water;
				position.y = randFloorHeight;
			}

			PGP_Generator::CreateCubeAndPushToList(cubeList, cubeType, position);
		}
	}
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

int PGP_Generator::GetDistance(glm::vec2 aPos, glm::vec2 bPos)
{
	return sqrt(pow(aPos.x - bPos.x, 2) + pow(aPos.y - bPos.y, 2));
}

Peak* PGP_Generator::GetClosestPeak(std::vector<Peak*> &peaks, glm::vec2 pos)
{
	Peak* closestPeak = peaks[0];
	int tempDistance = GetDistance(pos, closestPeak->position);

	for (int i = 1; i < peaks.size(); i++)
	{
		if (GetDistance(pos, peaks[i]->position) < tempDistance)
			closestPeak = peaks[i];
	}
	return closestPeak;
}