#include "PGP_Texture.h"
#include <stb_image.cpp>

PGP_Texture::PGP_Texture(const char* pathToFile, int new_textureID)
	: width(16), height(16), bpp(0), textureID(new_textureID), filepath(pathToFile), dataBuffer(nullptr)
{
	dataBuffer = PGP_Texture::LoadTextureData(pathToFile, width, height, bpp);
	InitTexture();
	stbi_image_free(dataBuffer);
}

unsigned char* PGP_Texture::LoadTextureData(const char* pathToFile, int& width, int& height, int& bpp)
{
	return stbi_load(pathToFile, &width, &height, &bpp, 4);
}

void PGP_Texture::InitTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void PGP_Texture::ActivateTexture(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
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
	glDeleteTextures(1, &textureID);
}