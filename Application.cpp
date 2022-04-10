#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PGP_EntityBase.h"
#include "PGP_EntityPrimitive.h"
#include "PGP_ShaderProgram.h"
#include "PGP_Camera.h"
#include "PGP_Texture.h"
#include "PGP_Generator.h"
#include <iostream>
#include <string>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

/*Containing all Cubes of project*/
std::vector<std::list<Cube*>> AllCubes;

void Update(GLFWwindow* window)
{
    glViewport(0, 0, 1920, 1280);

    /* Clear */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    PGP_Generator::DrawAllCubes(AllCubes);

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

    /*Initialize Shader Program and Camera*/
    GLuint program = PGP_ShaderProgram::CreateAndUseNewProgram();
    PGP_Camera* camera = new PGP_Camera(window, program, 45, glm::vec3(-5, 10, -10));

    /*Create 2D Cube Array*/
    AllCubes = std::vector<std::list<Cube*>>(ECubeTypeSize);
    PGP_Generator::InitializeAllCubesList(AllCubes);

    /*Procedural Generation*/
    PGP_Generator::CreateTerrain(AllCubes);

    std::cout << "Loaded after: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - initTimePoint).count() << "s" << endl;
    std::cout << "\nRUNNING..." << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window->p_window))
    {
        PGP_Time::UpdateTime();
        if (camera->UpdateCameraInput(window->p_window, program)) 
        {
            Update(window->p_window);
        }
        PGP_Time::SleepUntilFrameEnd();
    }
    /*Close Application*/
    glfwTerminate();
    //to-do: delete instances and buffer data

    return 0;
}