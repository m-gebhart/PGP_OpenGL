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

	Entity()
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
	glm::vec3 offsetToPivotPoint = glm::vec3(0.f);

	const static GLuint totalVertexByteSize = 8 * sizeof(float); // 8 = 4 pos + 4 col
	const static GLuint positionByteOffset = 0;
	const static GLuint colorByteOffset = 4 * sizeof(float);

public:
	Vertex() 
	{ 
	};
	~Vertex() {};

	Vertex(glm::vec4 initPosition)
	{
		position = initPosition;
	}

	void SetColor(glm::vec4 newColor)
	{
		color = newColor;
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

	static glm::vec4 Vec4(float inputArray[4]) 
	{
		return glm::vec4(inputArray[0], inputArray[1], inputArray[2], inputArray[3]);
	};

	static glm::vec4 Vec4(float inputArray[3], float wValue) 
	{
		return glm::vec4(inputArray[0], inputArray[1], inputArray[2], wValue);
	}
};