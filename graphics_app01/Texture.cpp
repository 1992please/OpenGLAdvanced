#include <string>
#include <GL\glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "Texture.h"

Texture::Texture(GLenum Target)
{
	mTarget = Target;
}

bool Texture::Load(const char* FileName)
{
	// load and generate the texture
	mFileName = FileName;
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(mFileName, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		switch (nrComponents)
		{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
		}

		// OpenGL shit here....
		glGenTextures(1, &mID);
		glBindTexture(mTarget, mID);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(mTarget, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(mTarget);
		glBindTexture(mTarget, 0);
	}
	else
	{
		mID = 0;
		std::cout << "Failed to load texture" << std::endl;
		return false;
	}
	stbi_image_free(data);
	return true;
}

void Texture::BindOffset(unsigned int index)
{
	assert(index < 32);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(mTarget, mID);
}

void Texture::Bind(GLenum index)
{
	glActiveTexture(index);
	glBindTexture(mTarget, mID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
}
