#include "cubemap_texture.h"
#include "stb_image.h"
#include <string.h>
static const GLenum sTypes[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

CubemapTexture::CubemapTexture(const char* PosXFilename, const char* NegXFilename, const char* PosYFilename, const char* NegYFilename, const char* PosZFilename, const char* NegZFilename)
{
	strcpy_s(mFileNames[0], PosXFilename);
	strcpy_s(mFileNames[1], NegXFilename);
	strcpy_s(mFileNames[2], PosYFilename);
	strcpy_s(mFileNames[3], NegYFilename);
	strcpy_s(mFileNames[4], PosZFilename);
	strcpy_s(mFileNames[5], NegZFilename);

	mTarget = GL_TEXTURE_CUBE_MAP;
}

CubemapTexture::~CubemapTexture()
{

}

bool CubemapTexture::Load()
{
	// load and generate the texture
	// Assuming you will give me jpeg files we will not flip
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, &mTextureID);
	glBindTexture(mTarget, mTextureID);
	bool bResult = true;
	for (unsigned int i = 0; i < 6; i++)
	{
		const char* mFileName = mFileNames[i];

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

			glTexImage2D(sTypes[i], 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			printf("Cube map texture failed to load at path: %s\n", mFileName);
			bResult = false;
		}
		stbi_image_free(data);
	}

	glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(mTarget, 0);

	return bResult;
}