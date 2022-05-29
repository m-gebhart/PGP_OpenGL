#pragma once
#include <list>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <chrono>
#include <gl/glew.h>
#include "PGP_EntityPrimitive.h"
#include "PGP_Time.h"
#include "PGP_Generator.h"

enum AnimationState {spawn, idle, clear};

class PGP_Renderer {
public:
	static void UpdateAndDrawCubes(std::vector<std::list<Cube*>> &cubes, GLuint textureSlot = 0, GLuint shaderProgram = 0);
	static void ClearRendering(void);

	static void StartDrawAnimation(void);
	static void StartClearAnimation(void);

	inline static AnimationState GetAnimState(void) { return animState; };

private:
	static void DrawCubes(std::vector<std::list<Cube*>> &cubes);
	static void InitCubeVerticesData(std::vector<std::list<Cube*>>& cubeList);
	static void UpdateCubeVerticesData(std::vector<std::list<Cube*>>& cubeList, int cubeCount, int cubeOffset = 0);
	static void InitCubeIndicesData(std::vector<std::list<Cube*>>&cubes);

	static std::unique_ptr<float[]> GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength, int verticesDataOffset = 0);
	static std::unique_ptr<int[]> GetIndicesData(std::vector<std::list<Cube*>>& cubes, int indicesDataLength);

	static int CalculateTotalCubeCount(std::vector<std::list<Cube*>>& cubes);
	static void DisableSurroundedCubes(void);

public:
	static int totalCubeCount;

private:
	/*buffer objects*/
	static GLuint vbo;
	static GLuint ibo;

	static bool bInitialized;
	static bool bReset;

	static int cubesToDraw;

	static AnimationState animState;
	static const int spawnAnimTime = 10;
	static const int clearAnimTime = 5;
	static float animTimer;
};