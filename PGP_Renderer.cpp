#include "PGP_Renderer.h"

void PGP_Renderer::UpdateAndDrawCubes(std::list<Cube*>& cubes, GLuint textureSlot, GLuint shaderProgram)
{
	if (!cubes.empty())
	{
		(*cubes.begin())->texture->ActivateTexture(0); //slot 0: main / albedo texture
		(*cubes.begin())->texture->SetUniformSlot(0, "textureSampler", 0);
		PGP_Renderer::UpdateCubesBufferData(cubes);
		PGP_Renderer::UpdateCubeIndicesBufferData(cubes);
		PGP_Renderer::DrawAllCubes(cubes);
	}
}

void PGP_Renderer::UpdateCubesBufferData(std::list<Cube*>& cubes)
{
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	float* verticesData = new float[Cube::totalDataSize * cubes.size()];

	unsigned int cubeCount = 0;
	for (Cube* cube : cubes)
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
	glBufferData(GL_ARRAY_BUFFER, Cube::totalDataSize * cubes.size() * sizeof(float), verticesData, GL_STATIC_DRAW);

	//0 = position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, 0);
	//1 = color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, (const void*)(Vertex::colorByteOffsetInBytes));
	//2 = uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::totalDataSizeInBytes, (const void*)(Vertex::UVByteOffsetInBytes));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const int cubeIndicesSize = 36;
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

void PGP_Renderer::UpdateCubeIndicesBufferData(std::list<Cube*>& cubes)
{
	//UpdateIndicesData
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	int* indices = new int[cubeIndicesSize * cubes.size()];

	for (unsigned int cube = 0; cube < cubes.size(); cube++)
	{
		for (int index = 0; index < cubeIndicesSize; index++)
			indices[cubeIndicesSize * cube + index] = cubeIndices[index] + cube * PGP_Primitives::Cube::totalVertexCount;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesSize * cubes.size() * sizeof(int), indices, GL_STATIC_DRAW);
}

void PGP_Renderer::DrawAllCubes(std::list<Cube*>& cubes)
{
	glDrawElements(GL_TRIANGLES, cubeIndicesSize * cubes.size(), GL_UNSIGNED_INT, nullptr);
}
