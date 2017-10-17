#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
class Texture
{
public:
	unsigned int ID;
	bool bLoaded;
	Texture();
	void LoadTexture(const char* texPath);
	void Bind(unsigned int index);
	~Texture();
};
#endif // !TEXTURE_H
