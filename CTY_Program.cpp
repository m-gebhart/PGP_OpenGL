#include "CTY_Program.h"

GLuint* CTY_Program::GetCurrentProgram()
{
    return currentProgram;
}

void CTY_Program::SetProgram(GLuint* newProgram)
{
	currentProgram = newProgram;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1280, 0.1f, 100.0f);
    glm::vec3 cameraPos = glm::vec3(0, 1, 6);
    glm::mat4 view = glm::lookAt(
        cameraPos,
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
}