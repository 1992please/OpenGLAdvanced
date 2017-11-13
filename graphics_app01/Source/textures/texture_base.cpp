#include "texture_base.h"
#include "assert.h"

TextureBase::TextureBase()
{
	mTarget = GL_TEXTURE_2D;
	mTextureID = 0;
}

TextureBase::~TextureBase()
{
	if (mTextureID != 0)
		glDeleteTextures(1, &mTextureID);
}

void TextureBase::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(mTarget, mTextureID);
}

void TextureBase::BindOffset(unsigned int index)
{
	assert(index < 32);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(mTarget, mTextureID);
}

