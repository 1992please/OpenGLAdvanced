#ifndef CUBEMAP_TEXTURE_H
#define CUBEMAP_TEXTURE_H
#include "texture_base.h"

class CubemapTexture :public TextureBase
{
public:
	CubemapTexture(
		const char* PosXFilename,
		const char* NegXFilename,
		const char* PosYFilename,
		const char* NegYFilename,
		const char* PosZFilename,
		const char* NegZFilename);

	~CubemapTexture();

	bool Load() override;

private:
	char mFileNames[6][255];
};

#endif
