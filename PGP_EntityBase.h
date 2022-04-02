#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

static int entityCounter = 0;

struct Entity {
	glm::vec3 pivotPointPosition;
	float scale;
	int entityID;

	Entity() : scale(1.f), pivotPointPosition(glm::vec3(0.f))
	{
		entityID = entityCounter++;
	}

	void SetPivotPoint(glm::vec3 newPivotPos)
	{
		pivotPointPosition = newPivotPos;
	}
};

struct Vertex {
public:
	glm::vec4 position = glm::vec4(0.f);
	glm::vec4 color = glm::vec4(0.5f);
	glm::vec2 uv = glm::vec2(0.5f);
	glm::vec3 offsetToPivotPoint = glm::vec3(0.f);

	const static GLuint totalVertexByteSize = 10 * sizeof(float); // 10 = 4 pos + 4 col + 2 uv
	const static GLuint positionByteOffset = 0;
	const static GLuint colorByteOffset = 4 * sizeof(float);
	const static GLuint UVByteOffset = 8 * sizeof(float);

public:
	Vertex(glm::vec4 initPosition)
	{
		position = initPosition;
	}

	void SetColor(glm::vec4 newColor)
	{
		color = newColor;
	}

	void SetUV(glm::vec2 newUVCoords)
	{
		uv = newUVCoords;
	}

	void SetPivotOffset(glm::vec3 newPivotOffset)
	{
		offsetToPivotPoint = newPivotOffset;
	}

	void SetPivotOffsetWithPivotPoint(glm::vec3 pivotPoint)
	{
		offsetToPivotPoint = glm::vec3(position) - pivotPoint;
	}
};

class PGP_Transform
{
	/*TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;*/
protected:
	static void ApplyMatrixOnVertexPosition(glm::mat4 transformMatrix, Vertex* vertex) 
	{
		glm::vec4 transformedVector = transformMatrix * vertex->position;
		glm::vec4 newPosition = glm::vec4(transformedVector.x, transformedVector.y, transformedVector.z, 1.0f);
		vertex->position = newPosition;
		//triangle->SetVertexPosition(vertexID, newPosition);
	};
};