#pragma once

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>

class PGP_Window
{
public:
	int				width;
	int				height;
	std::string		title;
	GLFWwindow*		p_window;

public:
	PGP_Window(int nWidth, int nHeight, std::string strTitle);
	~PGP_Window();

	void SetWindowData(int nWidth, int nHeight, std::string strTitle);
	bool Init(void);

	void InitGLSettings(void);

	bool IsRunning(void);
};