#include "PGP_EntityPrimitive.h"
#include <iostream>

Cube* PGP_EPrimitive::CreateCube(ECubeType cubeType, glm::vec3 centerPos, float scale, GLuint textureShaderProgram) 
{
	float colors[32]{
	1.f, 0.f, 0.f, 1.0f,
	1.f, 0.f, 0.f, 1.0f,
	1.f, 1.f, 0.25f, 1.0f,
	1.f, 1.f, 0.25f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,
	0.3f, 0.0f, 1.f, 1.0f,
	0.5f, 0.0f, 1.f, 1.0f
	};

	Cube* newCube = new Cube(centerPos, scale, cubeType, textureShaderProgram);

	for (int i = 0; i < PGP_Primitives::Cube::totalVertexCount; i++)
	{
		glm::vec4 vertexCol = glm::vec4(colors[i * 4],
			colors[i * 4 + 1],
			colors[i * 4 + 2],	
			colors[i*4 + 3]);

		newCube->vertices[i]->color = vertexCol;
	}
	PGP_EPrimitiveTransform::RotateCube(newCube, 180.f, glm::vec3(1.0f, 0, 0));

	return newCube;
}

float uvCoords[16] =
{
	1.f, 1.f, //0
	0.f, 1.f, //1
	0.f, 1.f, //2
	1.f, 1.f, //3
	1.f, 0.f, //4
	0.f, 0.f, //5
	0.f, 0.f, //6
	1.f, 0.f  //7
};

void Cube::InitializeCubeVerticesPositions(glm::vec3 centerPos, float newScale = 1.f)
{
	scale = newScale;

	float verticesPositions[32] =
	{
		centerPos.x - 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z - 0.5f * scale, 1.0f, //0 - left top back
		centerPos.x + 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z - 0.5f * scale, 1.0f, //1 - right top back
		centerPos.x - 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z + 0.5f * scale, 1.0f, //2 - left top front
		centerPos.x + 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z + 0.5f * scale, 1.0f, //3 - right top front
		centerPos.x - 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z - 0.5f * scale, 1.0f, //4 - left bottom back
		centerPos.x + 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z - 0.5f * scale, 1.0f, //5 - right bottom back
		centerPos.x - 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z + 0.5f * scale, 1.0f, //6 - left bottom front
		centerPos.x + 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z + 0.5f * scale, 1.0f  //7 - right bottom front
	};

	for (int i = 0; i < PGP_Primitives::Cube::totalVertexCount; i++) 
	{
		glm::vec4 vertexPos = glm::vec4(verticesPositions[i * 4],
			verticesPositions[i * 4 + 1],
			verticesPositions[i * 4 + 2],
			1.0f);

		vertices[i] = new Vertex(vertexPos);
		vertices[i]->SetUV(glm::vec2(uvCoords[i*2], uvCoords[i*2+1]));
	}
}

PGP_Texture* Cube::SetTexture(ECubeType cubeType)
{
	switch (cubeType) {
	case 0: /*ground*/
		texture = new PGP_Texture(".\\Ressources\\tile_ground.png", 0);
		break;
	case 1: /*water*/
		texture = new PGP_Texture(".\\Ressources\\tile_water.png", 1);
		break;
	case 2: /*water*/
		texture = new PGP_Texture(".\\Ressources\\tile_snow.png", 2);
		break;
		/*TO BE CONTNUED*/
	}
	return texture;
}

void PGP_EPrimitive::UpdateAndDrawCubes(std::list<Cube*> cubes, GLuint textureSlot, GLuint shaderProgram)
{
	if (!cubes.empty()) 
	{
		(*cubes.begin())->texture->ActivateTexture(0); //slot 0: main / albedo texture
		(*cubes.begin())->texture->SetUniformSlot(0, "textureSampler", 0);
		PGP_EPrimitive::UpdateCubesBufferData(cubes);
		PGP_EPrimitive::UpdateCubeIndicesBufferData(cubes);
		PGP_EPrimitive::DrawAllCubes(cubes);
	}
}

void PGP_EPrimitive::UpdateCubesBufferData(std::list<Cube*> cubes) 
{
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, Cube::totalByteSize * cubes.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	
	unsigned int cubeCount = 0;
	for (Cube* cube : cubes)
	{
		for (int vertex = 0; vertex < PGP_Primitives::Cube::totalVertexCount; vertex++)
		{
			float vertexPosData[4] = {
				cube->GetVertex(vertex)->position[0],
				cube->GetVertex(vertex)->position[1],
				cube->GetVertex(vertex)->position[2],
				cube->GetVertex(vertex)->position[3]
			};
			glBufferSubData(GL_ARRAY_BUFFER, cubeCount * Cube::totalByteSize + vertex * Vertex::totalVertexByteSize, sizeof(float) * 4, vertexPosData);

			float vertexColData[4] = {
				cube->GetVertex(vertex)->color[0],
				cube->GetVertex(vertex)->color[1],
				cube->GetVertex(vertex)->color[2],
				cube->GetVertex(vertex)->color[3]
			};
			glBufferSubData(GL_ARRAY_BUFFER, cubeCount * Cube::totalByteSize + vertex * Vertex::totalVertexByteSize + Vertex::colorByteOffset, sizeof(float) * 4, vertexColData);
		
			float vertexUVData[2] = { 
				cube->GetVertex(vertex)->uv[0],
				cube->GetVertex(vertex)->uv[1]
			};
			glBufferSubData(GL_ARRAY_BUFFER, cubeCount * Cube::totalByteSize + vertex * Vertex::totalVertexByteSize + Vertex::UVByteOffset, sizeof(float) * 2, vertexUVData);
		}
		cubeCount++;
	}
	
	//0 = position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::totalVertexByteSize, 0);
	//1 = color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::totalVertexByteSize, (const void*)(Vertex::colorByteOffset));
	//2 = uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::totalVertexByteSize, (const void*)(Vertex::UVByteOffset));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void PGP_EPrimitive::UpdateCubeIndicesBufferData(std::list<Cube*> cubes)
{
	//UpdateIndicesData
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * cubes.size() * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

	for (unsigned int cube = 0; cube < cubes.size(); cube++)
	{
		GLuint indices[36] = {};
		for (int index = 0; index < 36; index++)
			indices[index] = cubeIndices[index] + cube * PGP_Primitives::Cube::totalVertexCount;
		
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 36 * cube * sizeof(int), sizeof(int) * 36, indices);
	}
}

void PGP_EPrimitive::DrawAllCubes(std::list<Cube*> cubes)
{
	glDrawElements(GL_TRIANGLES, 36 * cubes.size(), GL_UNSIGNED_INT, nullptr);
}

void PGP_EPrimitiveTransform::TranslateCube(Cube* cube, glm::vec3 translationVector)
{
	glm::mat4 identityMatrix = glm::mat4(1);
	glm::mat4 translationMatrix = glm::translate(identityMatrix, translationVector);

	for (int vertexID = 0; vertexID < PGP_Primitives::Cube::totalVertexCount; vertexID++)
	{
		if (vertexID >= 1) {
			PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(translationMatrix, cube->GetVertex(vertexID));
			cube->GetVertex(vertexID)->SetPivotOffsetWithPivotPoint(cube->pivotPointPosition);
		}
		else
		{
			//cache pivot point offset on translation of first vertex
			glm::vec3 pivotOffset = glm::vec3(cube->vertices[vertexID]->position) - cube->pivotPointPosition;
			cube->vertices[vertexID]->SetPivotOffset(pivotOffset);

			//actual translation
			PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(translationMatrix, cube->GetVertex(vertexID));

			//set new pivot point
			glm::vec3 newPivotPos = glm::vec3(glm::vec3(cube->vertices[vertexID]->position) - pivotOffset);
			cube->SetPivotPoint(newPivotPos);
		}
	}
}

void PGP_EPrimitiveTransform::MoveCubeTo(Cube* cube, glm::vec3 targetPosition)
{
	glm::vec3 translationVector = targetPosition - cube->pivotPointPosition;
	TranslateCube(cube, translationVector);
}

void PGP_EPrimitiveTransform::RotateCube(Cube* cube, float degrees, glm::vec3 rotAxis)
{
	//set position back to origin (0,0,0) for matrix calculation
	glm::vec3 originalPosition = cube->pivotPointPosition;
	PGP_EPrimitiveTransform::MoveCubeTo(cube, glm::vec3(0));

	//actual rotation
	glm::mat4 rotationMatrix = glm::rotate(glm::radians(degrees), rotAxis);
	for (int vertexID = 0; vertexID < Cube::totalVertexCount; vertexID++)
		PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(rotationMatrix, cube->GetVertex(vertexID));

	//set back to previous position
	PGP_EPrimitiveTransform::MoveCubeTo(cube, originalPosition);
};

void PGP_EPrimitiveTransform::ScaleCube(Cube* cube, float newScale, bool bUpdateBuffer)
{
	cube->InitializeCubeVerticesPositions(cube->pivotPointPosition, newScale);
}

/*void PGP_EntityPrimitive::CreateShapes(void)
{
	float firstVerticesPos[12] = {
		-1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f };

	Triangle* firstTriangle = PGP_EntityPrimitive::CreateTriangle(firstVerticesPos);
	glm::vec4 one = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	firstTriangle->SetVertexColor(0, one);
	glm::vec4 two = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	firstTriangle->SetVertexColor(1, two);
	glm::vec4 three = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	firstTriangle->SetVertexColor(2, three);
	PGP_EntityPrimitive::UpdateAndBindBufferData();

	float secondVertices[12] = {
		1.0f, 0.0f, cos(45), 1.0f,
		-1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, cos(45) / 2, 1.0f };

	glm::vec4 secondColor = glm::vec4(
		0.0f, 0.0f, 1.0f, 1.0f
	);

	PGP_EntityPrimitive::CreateTriangle(secondVertices, secondColor);
	PGP_EntityPrimitive::UpdateAndBindBufferData();
}*/

/*
Triangle* PGP_EPrimitive::CreateTriangle(float verticesPosData[12], glm::vec4 colorData = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f))
{
	Triangle* newTriangle = new Triangle(verticesPosData);
	newTriangle->SetTriangleMonoColor(colorData);
	newTriangle->SetCenterAsPivotPoint();
	allTriangles.push_back(newTriangle);
	return allTriangles.back();
}

void PGP_EPrimitive::UpdateTriangleBufferData()
{
	GLuint vertex_buffer;
	GLsizei bufferSize = 1;
	glGenBuffers(bufferSize, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, Triangle::totalTriangleByteSize * allTriangles.size(), nullptr, GL_DYNAMIC_DRAW);

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	unsigned int triangleIndex = 0;
	for (Triangle* triangleData : allTriangles)
	{
		for (int vertex = 0; vertex < 3; vertex++)
		{
			float vertexPosData[4];
			for (int pos = 0; pos < 4; pos++)
				vertexPosData[pos] = triangleData->GetVertex(vertex)->position[pos];
			glBufferSubData(GL_ARRAY_BUFFER, triangleIndex * Triangle::totalTriangleByteSize + vertex * Vertex::totalVertexByteSize, sizeof(float) * 4, vertexPosData);

			float vertexColData[4];
			for (int col = 0; col < 4; col++)
				vertexColData[col] = triangleData->GetVertex(vertex)->color[col];
			glBufferSubData(GL_ARRAY_BUFFER, triangleIndex * Triangle::totalTriangleByteSize + vertex * Vertex::totalVertexByteSize + Vertex::colorByteOffset, sizeof(float) * 4, vertexColData);
		}
		triangleIndex++;
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::totalVertexByteSize, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::totalVertexByteSize, (const void*)(4 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PGP_EPrimitiveTransform::TranslateTriangle(Triangle* triangle, glm::vec3 translationVector)
{
	glm::mat4 identityMatrix = glm::mat4(1);
	glm::mat4 translationMatrix = glm::translate(identityMatrix, translationVector);

	for (int vertexID = 0; vertexID < 3; vertexID++)
	{
		if (vertexID < 2)
			PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(translationMatrix, triangle->GetVertex(vertexID));
		else
		{
			//cache pivot point offset on translation of last vertex
			glm::vec3 pivotOffset = glm::vec3(triangle->vertices[vertexID].position) - triangle->pivotPointPosition;
			triangle->vertices[vertexID].SetPivotOffset(pivotOffset);

			//actual translation
			PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(translationMatrix, triangle->GetVertex(vertexID));

			//set new pivot point
			glm::vec3 newPivotPos = glm::vec3(glm::vec3(triangle->vertices[vertexID].position) - pivotOffset);

			triangle->SetPivotPoint(newPivotPos);
		}
	}
}

void PGP_EPrimitiveTransform::RotateTriangle(Triangle* triangle, float degrees, glm::vec3 rotationAxis)
{
	//set position back to origin (0,0,0) for matrix calculation
	glm::vec3 originTranslationVec = glm::vec3(triangle->pivotPointPosition * -1.f);
	PGP_EPrimitiveTransform::TranslateTriangle(triangle, originTranslationVec);

	//actual rotation
	glm::mat4 rotationMatrix = glm::rotate(glm::radians(degrees), rotationAxis);
	for (int vertexID = 0; vertexID < 3; vertexID++)
	{
		PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(rotationMatrix, triangle->GetVertex(vertexID));
	}

	//set back to previous position
	glm::vec3 backToPreviousPos = glm::vec3(originTranslationVec * -1.f);
	PGP_EPrimitiveTransform::TranslateTriangle(triangle, backToPreviousPos);
}

void PGP_EPrimitiveTransform::ScaleTriangle(Triangle* triangle, glm::vec3 newScale)
{	//set position back to origin (0,0,0) for matrix calculation
	glm::vec3 originTranslationVec = glm::vec3(triangle->pivotPointPosition * -1.f);
	PGP_EPrimitiveTransform::TranslateTriangle(triangle, originTranslationVec);

	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1), newScale);
	for (int vertexID = 0; vertexID < 3; vertexID++)
	{
		PGP_EPrimitiveTransform::ApplyMatrixOnVertexPosition(scalingMatrix, triangle->GetVertex(vertexID));
	}

	//set back to previous position
	glm::vec3 backToPreviousPos = glm::vec3(originTranslationVec * -1.f);
	PGP_EPrimitiveTransform::TranslateTriangle(triangle, backToPreviousPos);
}*/