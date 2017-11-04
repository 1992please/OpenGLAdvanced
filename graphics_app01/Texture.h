#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL\glew.h>

enum ETextureColor
{
	TEXTURE_WHITE,
	TEXTURE_BLACK
};

class Texture
{
public:
	Texture(GLenum Target);
	Texture();
	bool Load(const char* FileName);
	void BindOffset(unsigned int index);
	void Bind(GLenum index);
	~Texture();

private:
	const char* mFileName;
	GLenum mTarget;
	unsigned int mID;
};
#endif // !TEXTURE_H
