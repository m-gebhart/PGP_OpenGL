#pragma once
#include "PGP_EntityBase.h"
#include "PGP_Texture.h"
#include <vector>
#include <list>
#include <iostream>

enum ECubeType {
	ground, //0
	water,  //1
	snow, 	//2
	sand, 	//3
	bush, 	//4
	dirt	//5
};
static const int ECubeTypeSize = 6;

namespace PGP_Primitives {

	struct Cube : public Entity {
	public:
		ECubeType type = ECubeType::ground;
		Vertex* vertices[8];
		const static GLuint totalVertexCount = 8;
		const static GLuint totalDataSize = 80; // totalVertexCount * Vertex's totalByteSize (10)
		const static GLuint totalDataSizeInBytes = 80 * sizeof(float); 
		PGP_Texture* texture;

	private:
		GLuint textureProgram;

	public:
		Cube(glm::vec3 centerPos, float initScale, ECubeType cubeType, GLuint textureShaderProgram) : Entity()
		{
			SetPivotPoint(centerPos);
			type = cubeType;
			textureProgram = textureShaderProgram;
			SetCubeVerticesPositions(centerPos, initScale);
			texture = SetTextureAndUVCoords(type);
		}

		Cube(const Cube& other)
		{
			SetPivotPoint(other.pivotPointPosition);
			type = other.type;
			textureProgram = other.textureProgram;
			SetCubeVerticesPositions(other.pivotPointPosition, other.scale);
			texture = SetTextureAndUVCoords(other.type);
		}

		void SetVertex(int vertexIndex, glm::vec4 vertexPosition, glm::vec4 vertexColor)
		{
			vertices[vertexIndex]->position = vertexPosition;
			vertices[vertexIndex]->color = vertexColor;
			vertices[vertexIndex]->SetPivotOffsetWithPivotPoint(pivotPointPosition);
		}

		Vertex* GetVertex(int index)
		{
			return vertices[index];
		}

		void SetCubeVerticesPositions(glm::vec3 centerPos, float scale);
		PGP_Texture* SetTextureAndUVCoords(ECubeType cubeType);
	};
};

using namespace PGP_Primitives;



class PGP_EPrimitive
{
public:
	/* --- CUBE ---*/
	static Cube* CreateCube(ECubeType cubetype, glm::vec3 centerPos, float scale = 1.f, GLuint textureShaderProgram = 0);
	/*--- PLANE ---*/
	//TBC
};

class PGP_EPrimitiveTransform : PGP_Transform
{
public:
	static void TranslateCube(Cube* cube, glm::vec3 translationVector);
	static void MoveCubeTo(Cube* cube, glm::vec3 targetPosition);
	static void RotateCube(Cube* cube, float degrees, glm::vec3 rotAxis);
	static void ScaleCube(Cube* cube, float newScale, bool bUpdateBuffer);
};