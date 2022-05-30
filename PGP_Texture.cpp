#include "PGP_Texture.h"
#include <stb_image.cpp>

unsigned char* PGP_Texture::dataBuffer;
int PGP_Texture::width;
int PGP_Texture::height;
int PGP_Texture::bpp;


unsigned char* PGP_Texture::LoadStaticTextureData(const char* pathToFile, int& width, int& height, int& bpp)
{
	return stbi_load(pathToFile, &width, &height, &bpp, 4);
}

void PGP_Texture::InitTexture(const char* pathToFile, int slot)
{
	dataBuffer = PGP_Texture::LoadStaticTextureData(pathToFile, width, height, bpp);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + slot);

	stbi_image_free(dataBuffer);
}

void PGP_Texture::DeactivateTextures()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void PGP_Texture::SetUniformSlot(GLuint program, const char* uniformName, int slot)
{
	GLuint textureSampler = glGetUniformLocation(program, uniformName);
	glUniform1i(textureSampler, slot);
}