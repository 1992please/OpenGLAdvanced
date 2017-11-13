#include <GL\glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include <string.h>

Texture::Texture(const char* FileName)
{
	mTarget = GL_TEXTURE_2D;
	if (FileName)
		strcpy_s(mFileName, FileName);
	else
		mFileName[0] = 0;
}

Texture::Texture()
{
	mFileName[0] = 0;
	mTarget = GL_TEXTURE_2D;

	int width = 1;
	int height = 1;
	GLenum format = GL_RGB;
	float data[] = {
		1, 1, 1 };
	glGenTextures(1, &mTextureID);
	glBindTexture(mTarget, mTextureID);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(mTarget, 0, GL_RGB, width, height, 0, format, GL_FLOAT, data);
	glBindTexture(mTarget, 0);
}

bool Texture::Load()
{
	Clear();
	// load and generate the texture
	// Assuming all textures will be png
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
		glGenTextures(1, &mTextureID);
		glBindTexture(mTarget, mTextureID);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(mTarget, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(mTarget);
		glBindTexture(mTarget, 0);
		stbi_image_free(data);
	}
	else
	{
		printf("Failed to load texture: %s\n", mFileName);
		return false;
	}
	return true;
}

Texture::~Texture()
{
}

void Texture::Clear()
{
	if (mTextureID != 0)
		glDeleteTextures(1, &mTextureID);
	mTextureID = 0;
}
