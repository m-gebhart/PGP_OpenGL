#include "PGP_Renderer.h"

bool PGP_Renderer::bInitialized = false;
bool PGP_Renderer::bReset = true;
const int cubeIndicesSize = 36;

GLuint PGP_Renderer::vbo;
GLuint PGP_Renderer::ibo;

void PGP_Renderer::UpdateAndDrawCubes(std::vector<std::list<Cube*>>& cubes, GLuint textureSlot, GLuint shaderProgram)
{
	if (!cubes.empty())
	{
		if (!bInitialized)
		{
			(*cubes[0].begin())->texture->SetUniformSlot(0, "textureSampler", 0);
			PGP_Renderer::BufferCubeVerticesData(cubes);
			PGP_Renderer::BufferCubeIndicesData(cubes);
			bInitialized = true;
		}

		PGP_Renderer::DrawCubes(cubes);
	}
}

void PGP_Renderer::BufferCubeVerticesData(std::vector<std::list<Cube*>> &cubes)
{
	DefineTotalCubeCount(cubes);
	int verticesDataCount = Cube::totalDataSize * PGP_Renderer::totalCubeCount;
	std::unique_ptr<float[]> verticesData = GetVerticesData(cubes, verticesDataCount);

	/*If first generation, allocate memory and buffer with glBufferData*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Cube::totalDataSize * PGP_Renderer::totalCubeCount * sizeof(float), verticesData.get(), GL_DYNAMIC_DRAW);

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

std::unique_ptr<float[]> PGP_Renderer::GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength)
{
	std::unique_ptr<float[]> verticesData = std::unique_ptr<float[]>(new float[verticesDataLength]);

	unsigned int cubeCount = 0;
	for (int i = 0; i < ECubeTypeSize; i++)
	{
		std::list<Cube*> cubesOfType = cubes[i];
		for (Cube* cube : cubesOfType)
		{
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

void PGP_Renderer::BufferCubeIndicesData(std::vector<std::list<Cube*>>& cubes)
{
	std::unique_ptr<int[]> indices = GetIndicesData(cubes, cubeIndicesSize * PGP_Renderer::totalCubeCount);

	/*If first generation, allocate memory and buffer with glBufferData*/
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesSize * PGP_Renderer::totalCubeCount * sizeof(int), indices.get(), GL_DYNAMIC_DRAW);
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
			for (int index = 0; index < cubeIndicesSize; index++)
				indices[cubeIndicesSize * cubeCount + index] = cubeIndices[index] + cubeCount * PGP_Primitives::Cube::totalVertexCount;
			cubeCount++;
		}
	}

	return indices;
}

void PGP_Renderer::DrawCubes(std::vector<std::list<Cube*>>& cubes)
{
	int check;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &check);

	if (check > 0) 
	{
		glDrawElements(GL_TRIANGLES, cubeIndicesSize * PGP_Renderer::totalCubeCount * sizeof(int), GL_UNSIGNED_INT, nullptr);
	}
}

int PGP_Renderer::totalCubeCount = 0;

int PGP_Renderer::DefineTotalCubeCount(std::vector<std::list<Cube*>>& cubes)
{
	PGP_Renderer::totalCubeCount = 0;
	for (int i = 0; i < ECubeTypeSize; i++)
		PGP_Renderer::totalCubeCount += cubes[i].size();
	return PGP_Renderer::totalCubeCount;
}

void PGP_Renderer::ClearRendering()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	bInitialized = false;
}
