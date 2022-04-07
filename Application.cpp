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
#include "PGP_Texture.h"
#include "PGP_Generator.h"
#include <iostream>
#include <string>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

/*Container of all Cubes*/
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
    PGP_Camera* camera = new PGP_Camera(window, program, 45, glm::vec3(0, 0, 4));

    /*Create 2D Cube Array*/
    AllCubes = std::vector<std::list<Cube*>>(ECubeTypeSize);
    PGP_Generator::InitializeAllCubesList(AllCubes);

    glm::vec3 cubeThreePos = glm::vec3(0.05f);
    Cube* cube1 = PGP_EPrimitive::CreateCube(ECubeType::ground, cubeThreePos, 0.15f);
    AllCubes[ECubeType::ground].push_back(cube1);

    glm::vec3 cubeTwoPos = glm::vec3(-1.0, -1.5f, -0.5f);
    Cube* cube2 = PGP_EPrimitive::CreateCube(ECubeType::snow, cubeTwoPos, 0.3f);
    AllCubes[ECubeType::snow].push_back(cube2);

    glm::vec3 cubeOnePos = glm::vec3(0.8f, 1.2f, 0);
    Cube* cube3 = PGP_EPrimitive::CreateCube(ECubeType::water, cubeOnePos, 0.7f);
    AllCubes[ECubeType::water].push_back(cube3);


    std::cout << "Loaded after: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - initTimePoint).count() << "s" << endl;
    std::cout << "\nRUNNING..." << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window->p_window))
    {
        PGP_Time::UpdateTime();
        if (camera->UpdateCameraInput(window->p_window, program)) 
        {
            Update(window->p_window);
            PGP_Time::SleepUntilFrameEnd();
        }
    }
    glfwTerminate();

    //to-do: delete instances and buffer data
    return 0;
}