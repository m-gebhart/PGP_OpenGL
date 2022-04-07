#include "PGP_Generator.h"

void PGP_Generator::InitializeAllCubesList(std::vector<std::list<Cube*>> emptyList)
{
	for (int i = 0; i < ECubeTypeSize; i++) 
	{
		static std::list<Cube*> newList = std::list<Cube*>();
		emptyList[i] = newList;
	}
}

void PGP_Generator::DrawAllCubes(std::vector<std::list<Cube*>> cubeList)
{
	for (int i = 0; i < cubeList.size(); i++)
		PGP_EPrimitive::UpdateAndDrawCubes(cubeList[i]);
}