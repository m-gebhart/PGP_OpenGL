#pragma once
#include "PGP_EntityBase.h"
#include "PGP_Texture.h"
#include <vector>
#include <list>
#include <iostream>

enum CubeType { ground, water, sand, bush };

namespace PGP_Primitives {

	struct Cube : public Entity {
	public:
		CubeType type = ground;
		Vertex* vertices[8];
		const static GLuint totalVertexCount = 8;
		const static GLuint totalByteSize = 80 * sizeof(float); // totalVertexCount * totalVertexSize (10)
	private:
		PGP_Texture* texture;
		GLuint textureProgram;

	public:
		Cube(glm::vec3 centerPos, float initScale, CubeType cubeType, GLuint textureShaderProgram)
		{
			SetPivotPoint(centerPos);
			type = cubeType;
			textureProgram = textureShaderProgram;
			texture = SetTexture(textureShaderProgram, type);
			InitializeCubeVerticesPositions(centerPos, initScale);
		}

		Cube(const Cube& other)
		{
			SetPivotPoint(other.pivotPointPosition);
			type = other.type;
			textureProgram = other.textureProgram;
			texture = SetTexture(textureProgram, other.type);
			InitializeCubeVerticesPositions(other.pivotPointPosition, other.scale);
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

		void InitializeCubeVerticesPositions(glm::vec3 centerPos, float scale);
		PGP_Texture* SetTexture(GLuint shaderProgram, CubeType cubeType);
	};

	/*struct Triangle : public Entity {
		Vertex vertices[3];
		const static GLuint totalTriangleByteSize = 24 * sizeof(float);

		Triangle(float initPosData[12])
		{
			InitAllVertices(initPosData);
		}

		void InitAllVertices(float initPosData[12])
		{
			for (int vertex = 0; vertex < 3; vertex++)
			{
				glm::vec4 vertexPosData;;
				for (int pos = 0; pos < 4; pos++)
					vertexPosData[pos] = initPosData[vertex * 4 + pos];
				InitVertex(vertex, vertexPosData);
			}
		}

		void InitVertex(int verticeID, glm::vec4 initPosition)
		{
			Vertex* newVertex = new Vertex(initPosition);
			newVertex->SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
			vertices[verticeID] = *newVertex;
		}

		void SetVertexPosition(int vertexID, glm::vec4 newPosition)
		{
			vertices[vertexID].position = newPosition;
		}

		void SetVertexColor(int vertexID, glm::vec4 newColor)
		{
			vertices[vertexID].color = newColor;
		}

		void SetCenterAsPivotPoint()
		{
			glm::vec3 center = glm::vec3((vertices[0].position.x + vertices[1].position.x + vertices[2].position.x) / 3.f,
				(vertices[0].position.y + vertices[1].position.y + vertices[2].position.y) / 3.f,
				(vertices[0].position.z + vertices[1].position.z + vertices[2].position.z) / 3.f);
			SetPivotPoint(center);
		}

		Vertex* GetVertex(int vertexID)
		{
			return &vertices[vertexID];
		}

		void SetTriangleMonoColor(glm::vec4 newColor)
		{
			for (int vertex = 0; vertex < 3; vertex++) {
				for (int i = 0; i < 4; i++)
					vertices[vertex].color = newColor;
			}
		}
	};*/
}

using namespace PGP_Primitives;

class PGP_EPrimitive
{
public:
	/* --- CUBE ---*/
	static std::list<Cube*> allCubes;

	static Cube* CreateCube(glm::vec3 centerPos, float scale, GLuint textureShaderProgram);
	static void UpdateAllCubesBufferData(void);
	static void DrawAllCubes(void);

protected:
	static void UpdateCubeIndicesBufferData(void);

public:
	/* --- TRIANGLE ---*/
	/*static std::list<Triangle*> allTriangles;

	static Triangle* CreateTriangle(float verticesData[12], glm::vec4 colorData);
	static void UpdateTriangleBufferData(void);*/
	//static void DrawAllTriangles();
};

class PGP_EPrimitiveTransform : PGP_Transform
{
public:
	static void TranslateCube(Cube* cube, glm::vec3 translationVector, bool bUpdateBuffer);
	static void MoveCubeTo(Cube* cube, glm::vec3 targetPosition, bool bUpdateBuffer);
	static void RotateCube(Cube* cube, float degrees, glm::vec3 rotAxis, bool bUpdateBuffer);
	static void ScaleCube(Cube* cube, float newScale, bool bUpdateBuffer);

	//static void TranslateTriangle(Triangle* triangle, glm::vec3 translationVector);
	//static void RotateTriangle(Triangle* triangle, float degrees, glm::vec3 rotAxis);
	//static void ScaleTriangle(Triangle* triangle, glm::vec3 newScale);
};