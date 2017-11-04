#include "material.h"
#include "texture.h"
#include "util.h"

Material::Material()
{
	mShininess = .5f;
	mSpecularIntensity = 0.0;
}

Material::~Material()
{
}

void Material::ColorChannel::LoadTexture(const char* TexPath)
{
	Texture* temp = new Texture(GL_TEXTURE_2D);
	if (TexPath && temp->Load(TexPath))
	{
		mTexture = temp;
	}
	else
	{
		SAFE_DELETE(temp);
		mTexture = new Texture();
	}
}

void Material::ColorChannel::LoadColor(float* Color)
{
	mColor.r = Color[0];
	mColor.g = Color[1];
	mColor.b = Color[2];
}

Material::ColorChannel::ColorChannel()
{
	mTexture = NULL;
	mColor = glm::vec3(1.0, 1.0, 1.0);
}

Material::ColorChannel::~ColorChannel()
{
	SAFE_DELETE(mTexture);
}
