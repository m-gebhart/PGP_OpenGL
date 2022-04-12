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
	static bool bInitialized;

public:
	PGP_Texture(const char* pathToFile);
	static unsigned char* LoadStaticTextureData(const char* pathToFile, int& width, int& height, int& bpp);
	void InitTexture(int slot);
	void DeactivateTextures();
	void SetUniformSlot(GLuint program, const char* uniformName, int slot);

	~PGP_Texture();
};
