/*NOTE TO MYSELF: Avoid Legacy OpenGL!
* LEGACY: glVertex*, glColor*, glNormal*, 
matrix stack/manipulation commands (glMatrixMode, LoadIdentity, Rotatef, Translatef),
glLight, glLightModel, glMaterial
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PGP_EntityBase.h"
#include "PGP_EntityPrimitive.h"
#include "PGP_ShaderProgram.h"
#include "PGP_Camera.h"
#include <iostream>
#include <string>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include "PGP_Texture.h"

void Update(GLFWwindow* window)
{
    glViewport(0, 0, 1920, 1280);

    /* Clear */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    PGP_EPrimitive::DrawAllCubes();
    //glDrawArrays(GL_TRIANGLES, 0, 3 * CTY_EntityBase::allTriangles.size());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

int main(void)
{
    std::cout << "Initializing..." << std::endl;

    std::chrono::time_point<std::chrono::steady_clock> initTimePoint = std::chrono::steady_clock::now();

    /* Initialize window */
    PGP_Window* window = new PGP_Window(PGP_Config::screenWidth, PGP_Config::screenHeight, "Procedural Generation");

    /* Initialize library */
    if (!glfwInit())
        return -1;
    std::cout << "GLFW: Initialized!" << std::endl;
    std::cout << "GL VERSION: " << glGetString(GL_VERSION) << std::endl;

    window->InitGLSettings();

    GLuint program = PGP_ShaderProgram::CreateAndUseNewProgram();
    PGP_Camera* camera = new PGP_Camera(window, program, 45, glm::vec3(0, 0, 4));

    glm::vec3 cubeThreePos = glm::vec3(0.05f);
    PGP_EPrimitive::CreateCube(cubeThreePos, 0.15f, program);
    glm::vec3 cubeTwoPos = glm::vec3(-1.0, -1.5f, -0.5f);
    PGP_EPrimitive::CreateCube(cubeTwoPos, 0.3f, program);
    glm::vec3 cubeOnePos = glm::vec3(0.8f, 1.2f, 0);
    Cube* rotatedCube = PGP_EPrimitive::CreateCube(cubeOnePos, 0.7f, program);

    std::cout << "Loaded after: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - initTimePoint).count() << "s" << endl;
    std::cout << "\nRUNNING..." << endl;

    //float rotationSpeed = 1.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window->p_window))
    {
        PGP_Time::UpdateTime();
        /* Transform Simulation - WIP Test / Purposes*/
        /*int rotationCount = 1;
        for (Cube* cube : PGP_EPrimitive::allCubes)
        {
           // PGP_EPrimitiveTransform::TranslateCube(cube, glm::vec3(0, 0.05f * PGP_Time::deltaTime, 0), true);
            //PGP_EPrimitiveTransform::MoveCubeTo(cube, cube->pivotPointPosition + glm::vec3(-1.f * PGP_Time::deltaTime), true);
           //glm::vec3 vec = glm::vec3(0.1f*PGP_Time::deltaTime);
           // PGP_EPrimitiveTransform::ScaleCube(cube, cube->scale + 0.001f, false);
            //PGP_EPrimitiveTransform::RotateCube(cube, rotationSpeed*rotationCount++, glm::vec3(0, 1.0f, 0), true);
        }*/

        PGP_EPrimitive::UpdateAllCubesBufferData();
        if (camera->UpdateCameraInput(window->p_window, program)) 
        {
            Update(window->p_window);
            PGP_Time::SleepUntilFrameEnd();
        }
    }
    glfwTerminate();

   // CTY_EntityBase::DeleteAllTriangles();
    return 0;
}