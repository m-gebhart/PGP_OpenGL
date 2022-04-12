#include "PGP_Texture.h"
#include <stb_image.cpp>

unsigned char* PGP_Texture::dataBuffer;
bool PGP_Texture::bInitialized = false;
int PGP_Texture::width;
int PGP_Texture::height;
int PGP_Texture::bpp;

PGP_Texture::PGP_Texture(const char* pathToFile) : filepath(pathToFile)
{
	if (!bInitialized)
	{
		dataBuffer = PGP_Texture::LoadStaticTextureData(pathToFile, width, height, bpp);
		InitTexture(0);
		stbi_image_free(dataBuffer);
		bInitialized = true;
	}
}

unsigned char* PGP_Texture::LoadStaticTextureData(const char* pathToFile, int& width, int& height, int& bpp)
{
	return stbi_load(pathToFile, &width, &height, &bpp, 4);
}

void PGP_Texture::InitTexture(int slot)
{
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

PGP_Texture::~PGP_Texture()
{
	//glDeleteTextures(1, &textureID);
}