#pragma once
#include <gl/glew.h>
#include <string>
#include <iostream>

class PGP_Texture
{
public:
	GLuint textureID;
private:
	const char* filepath;

	int height;
	int width;
	int bpp;

	static GLuint textureArray[];

	unsigned char* dataBuffer;

public:
	PGP_Texture(const char* pathToFile, int new_textureID);
	void InitTexture();
	void ActivateTexture(int slot);
	void DeactivateTextures();
	void SetUniformSlot(GLuint program, const char* uniformName, int slot);

	~PGP_Texture();
};
