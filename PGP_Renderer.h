#pragma once
#include <list>
#include <map>
#include <gl/glew.h>
#include "PGP_EntityPrimitive.h"


class PGP_Renderer {
public:
	static void UpdateAndDrawCubes(std::vector<std::list<Cube*>> &cubes, GLuint textureSlot = 0, GLuint shaderProgram = 0);
	static void ClearRendering(void);

private:
	static void DrawCubes(std::vector<std::list<Cube*>> &cubes);
	static void BufferCubeVerticesData(std::vector<std::list<Cube*>>& cubeList);
	static void BufferCubeIndicesData(std::vector<std::list<Cube*>>&cubes);
	static std::unique_ptr<float[]> GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength);
	static std::unique_ptr<int[]> GetIndicesData(std::vector<std::list<Cube*>>& cubes, int indicesDataLength);


	static int DefineTotalCubeCount(std::vector<std::list<Cube*>>& cubes);
	static bool bInitialized;
	static bool bReset;

	static GLuint vbo;
	static GLuint ibo;

public:
	static int totalCubeCount;
};