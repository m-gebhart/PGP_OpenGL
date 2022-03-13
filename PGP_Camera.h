#pragma once
#include "glm.hpp"
#include <GL/glew.h>
#include "ext.hpp"
#include <GLFW/glfw3.h>
#include "PGP_Time.h"
#include "PGP_Window.h"
#include "PGP_Config.h"
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

class PGP_Camera {
private: 
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

	int fov;
	float width;
	float height;
	glm::vec3 cameraPos;
	float camSpeed = 1.f;
	float mouseXSensitivity = 0.35f, mouseYSensitivity = 0.2f;
	glm::vec3 orientation;
	glm::mat4 MVP;

	GLFWwindow* window;

public:
	PGP_Camera(PGP_Window *gameWindow, GLuint program, int initFOV, glm::vec3 initCamPos);
	void UpdateCameraInput(GLFWwindow* window, GLuint program);
	void UpdateMouseInput(GLFWwindow* window, GLuint program);

protected:
	void SetView(GLuint program, glm::vec3 newCamPos, glm::vec3 newLookAtPos);
	void SetProjection(GLuint program, int newFOV, float newWidth, float newHeight);
	void SetModel(GLuint program, glm::mat4 newModel);
};