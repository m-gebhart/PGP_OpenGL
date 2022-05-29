#include "PGP_Renderer.h"

bool PGP_Renderer::bInitialized = false;
bool PGP_Renderer::bReset = true;
const int cubeIndicesSize = 36;

GLuint PGP_Renderer::vbo;
GLuint PGP_Renderer::ibo;

int PGP_Renderer::cubesToDraw = 0;
AnimationState PGP_Renderer::animState = AnimationState::idle;
float PGP_Renderer::animTimer = 0;

void PGP_Renderer::UpdateAndDrawCubes(std::vector<std::list<Cube*>>& cubes, GLuint textureSlot, GLuint shaderProgram)
{
	if (!cubes.empty())
	{
		/*Generating new Terrain*/
		if (!bInitialized)
		{
			(*cubes[0].begin())->texture->SetUniformSlot(0, "textureSampler", 0);
			InitCubeVerticesData(cubes);
			InitCubeIndicesData(cubes);
			bInitialized = true;
		}

		DrawCubes(cubes);
	}
}

void PGP_Renderer::InitCubeVerticesData(std::vector<std::list<Cube*>> &cubes)
{
	DisableSurroundedCubes();
	CalculateTotalCubeCount(cubes);

	//load empty buffer data to fill in later
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Cube::totalDataSize * PGP_Renderer::totalCubeCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

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

void PGP_Renderer::UpdateCubeVerticesData(std::vector<std::list<Cube*>>& cubes, int cubeCount, int cubeOffset)
{
	int verticesDataCount = Cube::totalDataSize * cubeCount;
	std::unique_ptr<float[]> verticesData = GetVerticesData(cubes, verticesDataCount);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, cubeOffset, Cube::totalDataSize * cubeCount * sizeof(float), verticesData.get());
}

std::unique_ptr<float[]> PGP_Renderer::GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength, int verticesDataOffset)
{
	std::unique_ptr<float[]> verticesData = std::unique_ptr<float[]>(new float[verticesDataLength]);

	unsigned int cubeCount = 0;
	for (int i = 0; i < ECubeTypeSize; i++)
	{
		std::list<Cube*> cubesOfType = cubes[i];
		for (Cube* cube : cubesOfType)
		{
			if (cubeCount < verticesDataOffset)
			{
				cubeCount++;
				continue;
			}

			if (!cube->bShouldRender)
				continue;

			if (cubeCount * Cube::totalDataSize >= verticesDataLength)
				return verticesData;

			for (int vertex = 0; vertex < PGP_Primitives::Cube::totalVertexCount; vertex++)
			{
				//load position from vertex
				for (int pos = 0; pos < 4; pos++)
				{
					verticesData[cubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + pos] = cube->GetVertex(vertex)->position[pos];
				}

				//load color from vertex
				for (int col = 0; col < 4; col++)
				{
					verticesData[cubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + Vertex::colorByteOffset + col] = cube->GetVertex(vertex)->color[col];
				}

				//load uv from vertex
				for (int uv = 0; uv < 2; uv++)
				{
					verticesData[cubeCount * Cube::totalDataSize + vertex * Vertex::totalDataSize + Vertex::UVByteOffset + uv] = cube->GetVertex(vertex)->uv[uv];
				}
			}
			cubeCount++;
		}
	}
	return verticesData;
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

void PGP_Renderer::InitCubeIndicesData(std::vector<std::list<Cube*>>& cubes)
{
	std::unique_ptr<int[]> renderIndices = GetIndicesData(cubes, cubeIndicesSize * PGP_Renderer::totalCubeCount);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesSize * PGP_Renderer::totalCubeCount * sizeof(int), renderIndices.get(), GL_DYNAMIC_DRAW);
}

std::unique_ptr<int[]> PGP_Renderer::GetIndicesData(std::vector<std::list<Cube*>>& cubes, int indicesDataLength)
{
	std::unique_ptr<int[]> indices = std::unique_ptr<int[]>(new int[cubeIndicesSize * PGP_Renderer::totalCubeCount]);

	unsigned int cubeCount = 0;
	for (int i = 0; i < ECubeTypeSize; i++)
	{
		std::list<Cube*> cubesOfType = cubes[i];
		for (Cube* cube : cubesOfType)
		{
			if (!cube->bShouldRender)
				continue;
			for (int index = 0; index < cubeIndicesSize; index++)
				indices[cubeIndicesSize * cubeCount + index] = cubeIndices[index] + cubeCount * PGP_Primitives::Cube::totalVertexCount;
			cubeCount++;
		}
	}

	return indices;
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
				int prevCubesDrawn = cubesToDraw;
				cubesToDraw = totalCubeCount - totalCubeCount * (animTimer / spawnAnimTime);
				UpdateCubeVerticesData(cubes, cubesToDraw);
			}
			else
				animState = AnimationState::idle;
		}
		else if (animState == AnimationState::clear)
		{
			if (animTimer > 0)
			{
				animTimer -= PGP_Time::deltaTime;
				cubesToDraw = std::min(cubesToDraw, totalCubeCount) * (animTimer/clearAnimTime);
			}
			else 
			{
				animState = AnimationState::idle;
				ClearRendering();
			}
		}
		else
			cubesToDraw = totalCubeCount;

		glm::clamp(cubesToDraw, 0, totalCubeCount);

		glDrawElements(GL_TRIANGLES, cubeIndicesSize * cubesToDraw * sizeof(int), GL_UNSIGNED_INT, nullptr);
	}
}

int PGP_Renderer::totalCubeCount = 0;

int PGP_Renderer::CalculateTotalCubeCount(std::vector<std::list<Cube*>>& cubes)
{
	PGP_Renderer::totalCubeCount = 0;
	for (int i = 0; i < cubes.size(); i++)
		for(Cube* cube : cubes[i])
			if (cube->bShouldRender)
				totalCubeCount++;

	return PGP_Renderer::totalCubeCount;
}

void PGP_Renderer::ClearRendering()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	bInitialized = false;
	PGP_Renderer::totalCubeCount = 0;
}

void PGP_Renderer::StartDrawAnimation()
{
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
	auto it = PGP_Generator::CubeDict.begin();
	for(it; it != PGP_Generator::CubeDict.end(); it++)
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
}