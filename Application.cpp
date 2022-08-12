#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PGP_EntityBase.h"
#include "PGP_EntityPrimitive.h"
#include "PGP_ShaderProgram.h"
#include "PGP_Camera.h"
#include "PGP_Texture.h"
#include "PGP_Generator.h"
#include "PGP_Renderer.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

/*Containing all Cubes of project*/
std::vector<std::list<Cube*>> AllCubes;
bool bTerrainGenerated;
bool bProcessing;

void Update(GLFWwindow* window)
{
    glViewport(0, 0, PGP_Config::screenWidth, PGP_Config::screenHeight);

    /* Clear */
    glClearColor(PGP_Config::skyBoxColor.x / 255.f, PGP_Config::skyBoxColor.y / 255.f, PGP_Config::skyBoxColor.z / 255.f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render */
    if (bTerrainGenerated)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        PGP_Renderer::UpdateAndDrawCubes(AllCubes);
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

void GenerateTerrain(std::vector<std::list<Cube*>>& cubeList)
{
    mutex mtx_terrainGeneration;
    mtx_terrainGeneration.lock();
    cubeList = std::vector<std::list<Cube*>>(ECubeTypeSize);
    PGP_Generator::CreateTerrain(AllCubes);
    PGP_Renderer::StartDrawAnimation();
    bProcessing = false;
    bTerrainGenerated = true;
    mtx_terrainGeneration.unlock();
}

void ResetTerrain()
{
    PGP_Renderer::StartClearAnimation();
}

int main(void)
{
    std::chrono::time_point<std::chrono::steady_clock> initTimePoint = std::chrono::steady_clock::now();

    std::cout << "Welcome to my Procedural Generation Project!";

    /* Initialize window */
    PGP_Window* window = new PGP_Window(PGP_Config::screenWidth, PGP_Config::screenHeight, "Procedural Generation");

    /* Initialize library */
    if (!glfwInit())
        return -1;
    std::cout << "GLFW: Initialized!\nGL VERSION: " << glGetString(GL_VERSION) << std::endl;


    window->InitGLSettings();

    PGP_Generator::terrainSize = PGP_Config::terrainSize;

    /*Initialize Shader Program, Textures and Camera*/
    GLuint program = PGP_ShaderProgram::CreateAndUseNewProgram();
    PGP_Texture::InitTextureLibrary(".\\Ressources\\Tiles\\tile_library.png", 0);
    PGP_Camera* camera = new PGP_Camera(window, program, 45, glm::vec3(PGP_Generator::terrainSize/2, 25, PGP_Generator::terrainSize+5));

    bTerrainGenerated = false;
    std::cout << "Loaded after: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - initTimePoint).count() << "s" << "\n\nRUNNING..." << std::endl;
    std::cout << "Press:\n -G to Generate\n -R to Reset\n -Esc to Escape" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window->p_window))
    {
        PGP_Time::UpdateTime();

        if (!bTerrainGenerated && !bProcessing && glfwGetKey(window->p_window, GLFW_KEY_G) == GLFW_PRESS && PGP_Renderer::GetAnimState() != AnimationState::spawn)
        {
            std::thread thrd(GenerateTerrain, std::ref(AllCubes));
            bProcessing = true;
            thrd.detach();
        }
        
        if (bTerrainGenerated && !bProcessing)
        {
            if (glfwGetKey(window->p_window, GLFW_KEY_R) == GLFW_PRESS && PGP_Renderer::GetAnimState() != AnimationState::clear)
                ResetTerrain();

            //if clear animation complete
            if (PGP_Renderer::drawnCubeCount == 0 && PGP_Renderer::GetAnimState() == AnimationState::idle && !AllCubes.empty())
            {
                PGP_Generator::ClearTerrain(AllCubes);
                bTerrainGenerated = false;
            }
        }

        if (camera->UpdateCameraInput(window->p_window, program)) 
            Update(window->p_window);

        PGP_Time::SleepUntilFrameEnd();
    }
    /*Close Application*/
    glfwTerminate();
    //to-do: delete instances and buffer data

    return 0;
}