#include "material.h"
#include "textures/texture.h"
#include "util.h"

Material::Material()
{
	mShininess = 32.f;
}

Material::~Material()
{
}

void Material::ColorChannel::LoadTexture(const char* TexPath)
{
	Clear();
	Texture* temp = new Texture(TexPath);
	if (TexPath && temp->Load())
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

void Material::ColorChannel::Clear()
{
	SAFE_DELETE(mTexture);
}

Material::ColorChannel::ColorChannel()
{
	mTexture = new Texture();
	mColor = glm::vec3(1.0, 1.0, 1.0);
}

Material::ColorChannel::~ColorChannel()
{
	Clear();
}
