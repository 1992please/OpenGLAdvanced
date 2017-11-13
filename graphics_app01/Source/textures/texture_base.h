#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H
#include <GL\glew.h>

class TextureBase
{
public:
	TextureBase();
	~TextureBase();
	virtual bool Load() {return false;}
	void Bind(GLenum TextureUnit);
	void BindOffset(unsigned int index);
protected:
	GLenum mTarget;
	unsigned int mTextureID;
};

#endif
