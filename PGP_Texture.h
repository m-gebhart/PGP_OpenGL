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

	unsigned char* dataBuffer;

public:
	PGP_Texture(const char* pathToFile, int texture_id);
	void LoadTexture();
	void ActivateTexture(int slot);
	void DeactivateTextures();
	void SetUniformSlot(GLuint program, const char* uniformName, int slot);

	~PGP_Texture();
};
