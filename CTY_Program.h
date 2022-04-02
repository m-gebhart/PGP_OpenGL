#include <GL/glew.h>
#include <glm.hpp>
#include <ext.hpp>

class CTY_Program
{
public:
	static GLuint* currentProgram;
public:
	 static GLuint* GetCurrentProgram(void);
	 static void SetProgram(GLuint* newProgram);
};