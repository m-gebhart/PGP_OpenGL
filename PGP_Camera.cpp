#include "PGP_Camera.h"

PGP_Camera::PGP_Camera(PGP_Window* gameWindow, GLuint initProgram, int initFOV, glm::vec3 initCamPos)
{
    window = gameWindow->p_window;
    SetProjection(initProgram, initFOV, gameWindow->width, gameWindow->height);
    SetView(initProgram, initCamPos, initCamPos + glm::vec3(0, 0, -1.f));
    SetModel(initProgram, glm::mat4(1.0f));

    glfwSetCursorPos(window, width / 2, height / 2);
   // glm::mat4 mvp = projection * view * model;
}

float rot = 0;

bool PGP_Camera::UpdateCameraInput(GLFWwindow* window, GLuint program)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, 1);
        return false;
    }

    glm::vec3 translationVector = glm::vec3(0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        translationVector += (orientation-cameraPos) * camSpeed * PGP_Time::deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        translationVector -= (orientation - cameraPos) * camSpeed * PGP_Time::deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
       glm::vec3 left = glm::cross(orientation - cameraPos, glm::vec3(0, -1, 0));
       translationVector += left * camSpeed * PGP_Time::deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        glm::vec3 right = glm::cross(glm::vec3(0, -1, 0), orientation - cameraPos);
        translationVector += right * camSpeed * PGP_Time::deltaTime;
    }

    if (translationVector != glm::vec3(0))
        SetView(program, cameraPos + translationVector, orientation + translationVector);

    UpdateMouseInput(window, program);
    return true;
}

void PGP_Camera::UpdateMouseInput(GLFWwindow* window, GLuint program)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    float horizontalAngle = (width / 2) - (float)xPos;
    glm::mat3 horizontalRotation = glm::rotate(glm::radians(horizontalAngle * mouseXSensitivity), glm::vec3(0, 1.f, 0));

    float verticalAngle = (height / 2) - (float)yPos;
    glm::mat3 verticalRotation = glm::rotate(glm::radians(verticalAngle * mouseYSensitivity), glm::vec3(1.f, 0, 0));

    SetView(program, cameraPos, (horizontalRotation * verticalRotation * glm::vec3(0, 0, -1.f)) + cameraPos);
}

void PGP_Camera::SetProjection(GLuint program, int newFOV, int newWidth, int newHeight)
{
    fov = newFOV;
    width = newWidth;
    height = newHeight;
	GLuint ProjectionID = glGetUniformLocation(program, "P");
	projection = glm::perspective(glm::radians((float)fov), (float)(width / height), 0.1f, 100.0f);
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &projection[0][0]);
}

void PGP_Camera::SetView(GLuint program, glm::vec3 newCamPos, glm::vec3 newLookAtPos)
{
    cameraPos = newCamPos;
    orientation = newLookAtPos;
    GLuint viewID = glGetUniformLocation(program, "V");
    view = glm::lookAt(
        newCamPos,
        newLookAtPos,
        glm::vec3(0, 1, 0) //head = up
    );
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}

void PGP_Camera::SetModel(GLuint program, glm::mat4 newModel)
{
    GLuint modelID = glGetUniformLocation(program, "M");
    model = newModel;
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}