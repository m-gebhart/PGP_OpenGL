#include "PGP_EntityPrimitive.h"

Cube* PGP_EPrimitive::CreateCube(ECubeType cubeType, glm::vec3 centerPos, float scale, GLuint textureShaderProgram) 
{
	Cube* newCube = new Cube(centerPos, scale, cubeType, textureShaderProgram);
	PGP_EPrimitiveTransform::RotateCube(newCube, 180.f, glm::vec3(1.f, 0, 0));

	return newCube;
}

void Cube::SetCubeVerticesPositions(glm::vec3 centerPos, bool bCreateVertices, float newScale = 1.f)
{
	scale = newScale;

	float verticesPositions[24] =
	{
		centerPos.x - 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z - 0.5f * scale, //0 - left top back
		centerPos.x + 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z - 0.5f * scale, //1 - right top back
		centerPos.x - 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z + 0.5f * scale, //2 - left top front
		centerPos.x + 0.5f * scale, centerPos.y + 0.5f * scale, centerPos.z + 0.5f * scale, //3 - right top front
		centerPos.x - 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z - 0.5f * scale, //4 - left bottom back
		centerPos.x + 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z - 0.5f * scale, //5 - right bottom back
		centerPos.x - 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z + 0.5f * scale, //6 - left bottom front
		centerPos.x + 0.5f * scale, centerPos.y - 0.5f * scale, centerPos.z + 0.5f * scale //7 - right bottom front		
	};

	for (int i = 0; i < PGP_Primitives::Cube::totalVertexCount; i++)
	{
		//first sides, then top and bottom: 0-7 for sides, 0-3 for top, 4-7 for bottom
		int offset = i * 3;
		if (offset >= 24)
			offset -= 24;

		glm::vec4 vertexPos = glm::vec4(verticesPositions[offset],
				verticesPositions[offset + 1],
				verticesPositions[offset + 2],
				1.f);

		if (bCreateVertices)
			vertices[i] = new Vertex(vertexPos);
		else
			vertices[i]->position = vertexPos;
	}
}

PGP_Texture* Cube::SetTextureAndUVCoords(ECubeType cubeType)
{
	texture = new PGP_Texture();

	float uvCoords[32] =
	{
		//SIDES
		1.f, 0.66f, //0 (VertexNumber)
		0.f, 0.66f, //1
		0.f, 0.66f, //2
		1.f, 0.66f, //3
		1.f, 0.33f, //4
		0.f, 0.33f, //5
		0.f, 0.33f, //6
		1.f, 0.33f, //7

		//TOP
		0.f, 1.f, //8
		1.f, 1.f, //9
		0.f, 0.66f, //10
		1.f, 0.66f, //11

		//BOTTOM
		0.f, 0.f, //12
		1.f, 0.f, //13
		0.f, 0.33f, //14
		1.f, 0.33f  //15
	};


	for (int i = 0; i < PGP_Primitives::Cube::totalVertexCount; i++)
	{
		//setting x coord to read from right tile from PNG tile library (sorted after cubeType)
		uvCoords[i * 2] = (cubeType + uvCoords[i * 2]) * (1.f / ECubeTypeSize);
		vertices[i]->SetUV(glm::vec2(uvCoords[i * 2], uvCoords[i * 2 + 1]));
	}

	return texture;
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

void PGP_EPrimitiveTransform::ScaleCube(Cube* cube, float newScale)
{
	cube->SetCubeVerticesPositions(cube->pivotPointPosition, false, newScale);
}