#pragma once
#include <gl/glew.h>
#include <string>
#include <iostream>

class PGP_Texture
{
private:
	const char* filepath;

	static int height;
	static int width;
	static int bpp;

	static unsigned char* dataBuffer;

public:
	static unsigned char* LoadStaticTextureData(const char* pathToFile, int& width, int& height, int& bpp);
	static void InitTexture(const char* pathToFile, int slot);
	void DeactivateTextures();
	void SetUniformSlot(GLuint program, const char* uniformName, int slot);
};
