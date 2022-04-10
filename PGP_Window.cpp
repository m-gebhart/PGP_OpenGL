#include "PGP_Window.h"
#include <iostream>

PGP_Window::PGP_Window(int newWidth, int newHeight, std::string newTitle)
{
	SetWindowData(newWidth, newHeight, newTitle);
	if(!Init()) 
		std::cout << "GLFW_Window: Error while Initializing!" << std::endl;
}

PGP_Window::~PGP_Window()
{
	glfwDestroyWindow(p_window);
	glfwTerminate();
}

void PGP_Window::SetWindowData(int newWidth, int newHeight, std::string newTitle)
{
	title = newTitle;
	width = newWidth;
	height = newHeight;
}

bool PGP_Window::Init(void)
{
	//glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit()) 
		return false;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	p_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!p_window) {
		glfwTerminate();
		return false;
	}

	glfwGetFramebufferSize(p_window, &width, &height);
	//glfwSetKeyCallback(p_window, KeyCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(p_window);
	glfwSwapInterval(1);

	if (glewInit() == GLEW_OK)
		std::cout << "GLEW: Initialized!" << std::endl;
	else
		std::cout << "GLEW: Error while Initializing!" << std::endl;

	return true;
}

void PGP_Window::InitGLSettings()
{
	//glClearDepth(10.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/* Culling */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	/*Alpha*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool PGP_Window::IsRunning(void)
{
	return !glfwWindowShouldClose(p_window);
}
