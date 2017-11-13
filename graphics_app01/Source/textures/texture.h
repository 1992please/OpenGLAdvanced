#ifndef TEXTURE_H
#define TEXTURE_H
#include "texture_base.h"

class Texture : public TextureBase
{
public:
	Texture(const char* FileName);
	Texture();
	bool Load() override;
	~Texture();
private:
	void Clear();
	char mFileName[255];
};

#endif // !TEXTURE_H
