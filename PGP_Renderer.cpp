#include "PGP_Renderer.h"

bool PGP_Renderer::bInitialized = false;
bool PGP_Renderer::bReset = true;
bool PGP_Renderer::bHollowTerrainSet = false;
const int cubeIndicesSize = 36;

GLuint PGP_Renderer::vbo;
GLuint PGP_Renderer::ibo;

int PGP_Renderer::drawnCubeCount = 0;
AnimationState PGP_Renderer::animState = AnimationState::idle;
float PGP_Renderer::animTimer = 0;

std::unique_ptr<float[]> PGP_Renderer::verticesData(new float[1000]);
std::unique_ptr<int[]> PGP_Renderer::indicesData(new int[1000]);


void PGP_Renderer::UpdateAndDrawCubes(std::vector<std::list<Cube*>>& cubes, GLuint textureSlot, GLuint shaderProgram)
{
	if (!cubes.empty())
	{
		/*Generating new Terrain*/
		if (!bInitialized)
		{
			InitVerticesData(cubes, PGP_Generator::CubeDict.size());
			InitVerticesBuffer();
			InitIndicesData(cubes, PGP_Generator::CubeDict.size());
			InitIndicesBuffer();

			(*cubes[0].begin())->texture->SetUniformSlot(0, "textureSampler", 0);
			bInitialized = true;

			UpdateVerticesBuffer(verticesData, PGP_Generator::CubeDict.size());

		}

		DrawCubes(cubes);
	}
}

int PGP_Renderer::renderCubeCount = 0;


void PGP_Renderer::InitVerticesBuffer()
{
	//load empty buffer data to fill in later
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Cube::totalDataSize * PGP_Generator::CubeDict.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	//0 = position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, 0);
	//1 = color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, (const void*)(Vertex::colorByteOffsetInBytes));
	//2 = uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, (const void*)(Vertex::UVByteOffsetInBytes));
}

void PGP_Renderer::InitVerticesData(std::vector<std::list<Cube*>>& cubes, int cubeCount)
{
	verticesData = GetVerticesData(cubes, Cube::totalDataSize * cubeCount);
}

void PGP_Renderer::UpdateVerticesBuffer(std::unique_ptr<float[]>& bufferData, int cubeCount, int bufferOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, Cube::totalDataSize * cubeCount * sizeof(float), bufferData.get());
}

std::unique_ptr<float[]> PGP_Renderer::GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength)
{
	//while buffering cubes of type 0-4, check which dirt cubes are not needed to render
	std::unique_ptr<std::thread> thread;
	if (!bHollowTerrainSet) 
		thread = std::unique_ptr<std::thread>(new std::thread(DisableSurroundedCubes));

	std::unique_ptr<float[]> tempVerticesData = std::unique_ptr<float[]>(new float[verticesDataLength]);

	PGP_Renderer::renderCubeCount = 0;
	unsigned int arrayCubeCount = 0;
	for (int i = 0; i < cubes.size(); i++)
	{
		std::list<Cube*> cubesOfType = cubes[i];

		if (i == cubes.size() - 1 && thread.get() != nullptr)
			thread->join(); //before getting data from cubes of type dirt (ID: 5), wait until hollow calculation is done

		for (Cube* cube : cubesOfType)
		{
			if (arrayCubeCount * Cube::totalDataSize >= verticesDataLength) {
				return tempVerticesData;
			}

			if (cube->bShouldRender)
			{
				renderCubeCount++;

				for (int vertex = 0; vertex < PGP_Primitives::Cube::totalVertexCount; vertex++)
				{
					//load position from vertex
					for (int pos = 0; pos < 4; pos++)
					{
						tempVerticesData[arrayCubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + pos] = cube->GetVertex(vertex)->position[pos];
					}

					//load color from vertex
					for (int col = 0; col < 4; col++)
					{
						tempVerticesData[arrayCubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + Vertex::colorByteOffset + col] = cube->GetVertex(vertex)->color[col];
					}

					//load uv from vertex
					for (int uv = 0; uv < 2; uv++)
					{
						tempVerticesData[arrayCubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + Vertex::UVByteOffset + uv] = cube->GetVertex(vertex)->uv[uv];
					}
				}
				arrayCubeCount++;
			}
		}
	}
	return tempVerticesData;
}

GLuint cubeIndices[36] =
{
	0, 1, 3, //top
	0, 3, 2,
	1, 0, 4, //front
	1, 4, 5,
	4, 6, 7, //bottom
	4, 7, 5,
	3, 1, 5, //right
	3, 5, 7,
	2, 6, 4, //left
	2, 4, 0,
	6, 2, 3, //back
	6, 3, 7
};

void PGP_Renderer::InitIndicesData(std::vector<std::list<Cube*>>& cubes, int cubeCount)
{
	indicesData = GetIndicesData(cubes, cubeIndicesSize * cubeCount);
}

std::unique_ptr<int[]> PGP_Renderer::GetIndicesData(std::vector<std::list<Cube*>>& cubes, int indicesDataLength)
{
	std::unique_ptr<int[]> tempIndicesData = std::unique_ptr<int[]>(new int[indicesDataLength]);

	unsigned int cubeCount = 0;
	for (int i = 0; i < cubes.size(); i++)
	{
		std::list<Cube*> cubesOfType = cubes[i];
		for (Cube* cube : cubesOfType)
		{
			if (!cube->bShouldRender)
				continue;
			for (int index = 0; index < cubeIndicesSize; index++) 
			{
				tempIndicesData[cubeIndicesSize * cubeCount + index] = cubeIndices[index] + cubeCount * PGP_Primitives::Cube::totalVertexCount;
			}
			cubeCount++;
		}
	}

	return tempIndicesData;
}

void PGP_Renderer::InitIndicesBuffer()
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesSize * PGP_Renderer::renderCubeCount * sizeof(int), indicesData.get(), GL_DYNAMIC_DRAW);
}

void PGP_Renderer::DrawCubes(std::vector<std::list<Cube*>>& cubes)
{
	int isValid;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &isValid);

	if (isValid > 0)
	{
		if (animState == AnimationState::spawn)
		{
			if (animTimer > 0)
			{
				animTimer -= PGP_Time::deltaTime;
				drawnCubeCount = PGP_Renderer::renderCubeCount - PGP_Renderer::renderCubeCount * (animTimer / spawnAnimTime);
				UpdateVerticesBuffer(verticesData, drawnCubeCount);
			}
			else
				animState = AnimationState::idle;
		}
		else if (animState == AnimationState::clear)
		{
			if (animTimer > 0 && drawnCubeCount > 0)
			{
				animTimer -= PGP_Time::deltaTime;
				drawnCubeCount = std::min(drawnCubeCount, renderCubeCount) * (animTimer / clearAnimTime);
			}
			else
			{
				animState = AnimationState::idle;
				ClearRendering();
			}
		}
		else
			drawnCubeCount = renderCubeCount;

		glm::clamp(drawnCubeCount, 0, renderCubeCount);

		glDrawElements(GL_TRIANGLES, cubeIndicesSize * drawnCubeCount * sizeof(int), GL_UNSIGNED_INT, nullptr);
	}
}

void PGP_Renderer::ClearRendering()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	bInitialized = false;
	bHollowTerrainSet = false;
	PGP_Renderer::renderCubeCount = 0;
}

void PGP_Renderer::StartDrawAnimation()
{
	drawnCubeCount = PGP_Generator::CubeDict.size()-1;
	animTimer = spawnAnimTime;
	animState = AnimationState::spawn;
}

void PGP_Renderer::StartClearAnimation()
{
	animTimer = clearAnimTime;
	animState = AnimationState::clear;
}

void PGP_Renderer::DisableSurroundedCubes()
{
	std::unordered_map<std::tuple<int, int, int>, PGP_Primitives::Cube*, custom_hash>::iterator it = PGP_Generator::CubeDict.begin();
	for (it; it != PGP_Generator::CubeDict.end(); it++)
	{
		//set invisible if surrounded by cubes (hollow)
		glm::vec3 position = it->second->pivotPointPosition;
		if (PGP_Generator::CubeDict.count(std::make_tuple(position.x - 1, position.y, position.z)) > 0
			&& PGP_Generator::CubeDict.count(std::make_tuple(position.x + 1, position.y, position.z)) > 0
			&& PGP_Generator::CubeDict.count(std::make_tuple(position.x, position.y - 1, position.z)) > 0
			&& PGP_Generator::CubeDict.count(std::make_tuple(position.x, position.y + 1, position.z)) > 0
			&& PGP_Generator::CubeDict.count(std::make_tuple(position.x, position.y, position.z - 1)) > 0
			&& PGP_Generator::CubeDict.count(std::make_tuple(position.x, position.y, position.z + 1)) > 0)
			it->second->bShouldRender = false;
	}

	bHollowTerrainSet = true;
}