#include "PGP_Texture.h"
#include <stb_image.cpp>

PGP_Texture::PGP_Texture(const char* pathToFile, int texture_id) 
	: width(16), height(16), bpp(0), textureID(texture_id), filepath(pathToFile), dataBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	dataBuffer = stbi_load(pathToFile, &width, &height, &bpp, 4);
	LoadTexture();
}

void PGP_Texture::LoadTexture()
{
	ActivateTexture(0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataBuffer);
	
	glGenerateMipmap(GL_TEXTURE_2D);
}

void PGP_Texture::ActivateTexture(int slot)
{
	glGenTextures(1, &textureID);
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