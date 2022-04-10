#pragma once
#include <list>
#include <gl/glew.h>
#include "PGP_EntityPrimitive.h"

class PGP_Renderer {
public:
	static void UpdateAndDrawCubes(std::list<Cube*>& cubes, GLuint textureSlot = 0, GLuint shaderProgram = 0);

private:
	static void UpdateCubesBufferData(std::list<Cube*>& cubes);
	static void DrawAllCubes(std::list<Cube*>& cubes);
	static void UpdateCubeIndicesBufferData(std::list<Cube*>& cubes);
};