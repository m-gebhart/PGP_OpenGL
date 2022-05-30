#pragma once
#include <list>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <chrono>
#include <gl/glew.h>
#include <thread>
#include "PGP_EntityPrimitive.h"
#include "PGP_Time.h"
#include "PGP_Generator.h"

enum AnimationState { spawn, idle, clear };

class PGP_Renderer {
public:
	static void UpdateAndDrawCubes(std::vector<std::list<Cube*>>& cubes, GLuint textureSlot = 0, GLuint shaderProgram = 0);
	static void ClearRendering(void);

	static void StartDrawAnimation(void);
	static void StartClearAnimation(void);

	inline static AnimationState GetAnimState(void) { return animState; };

private:
	static void DrawCubes(std::vector<std::list<Cube*>>& cubes);
	static void InitVerticesData(std::vector<std::list<Cube*>>& cubes, int cubeCount);
	static void InitVerticesBuffer();
	static void UpdateVerticesBuffer(std::unique_ptr<float[]>& bufferData, int cubeCount, int bufferOffset = 0);
	static void InitIndicesData(std::vector<std::list<Cube*>>& cubes, int cubeCount);
	static void InitIndicesBuffer();

	static std::unique_ptr<float[]> GetVerticesData(std::vector<std::list<Cube*>>& cubes, int verticesDataLength);
	static std::unique_ptr<int[]> GetIndicesData(std::vector<std::list<Cube*>>& cubes, int indicesDataLength);

	static void DisableSurroundedCubes(void);

public:
	static int renderCubeCount;

private:
	/*chunk data*/
	static std::unique_ptr<float[]> verticesData;
	static std::unique_ptr<int[]> indicesData;

	/*buffer objects*/
	static GLuint vbo;
	static GLuint ibo;

	static bool bInitialized;
	static bool bReset;
	static bool bHollowTerrainSet;

	static int cubesToDraw;

	static AnimationState animState;
	static const int spawnAnimTime = 16;
	static const int clearAnimTime = 5;
	static float animTimer;
};