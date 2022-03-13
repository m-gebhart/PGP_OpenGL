#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <ext.hpp>
#include "PGP_Camera.h"

using namespace std;

class PGP_ShaderProgram
{
public:
	static std::string vertexShader;
	static std::string fragmentShader;

public:
	static GLuint CreateAndUseNewProgram(void);

private:
	static void ReadFile(const std::string fileName, std::string& output);
	static GLuint CreateShader(GLuint shaderType, const std::string& sourceText);
	static bool CheckForError(int shaderID);
	static GLuint CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
	static void UseProgram(GLuint program);
};