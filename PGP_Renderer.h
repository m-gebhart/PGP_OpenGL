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
	static void InitCubeVerticesBufferData(std::vector<std::list<Cube*>>& cubeList);
	static void InitCubeIndicesBufferData(std::vector<std::list<Cube*>>&cubes);
	static std::unique_ptr<float[]> GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength);

	static int CalculateTotalCubeCount(std::vector<std::list<Cube*>>& cubes);
	static bool bInitialized;
	static bool bReset;

public:
	static int totalCubeCount;
};