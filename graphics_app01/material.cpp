#include "material.h"
#include "texture.h"
#include "util.h"

Material::Material()
{
	mSpecularPower = 32;
	mSpecularIntensity = 0.0;
}

Material::~Material()
{
}

void Material::ColorChannel::LoadTexture(const char* TexPath)
{
	if (!TexPath)
	{
		return;
	}
	Texture* temp = new Texture(GL_TEXTURE_2D);
	if (temp->Load(TexPath))
	{
		mTexture = temp;
	}
	else
	{
		SAFE_DELETE(temp);
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
	mColor = glm::vec3(0.0, 0.0, 0.0);
}

Material::ColorChannel::~ColorChannel()
{
	SAFE_DELETE(mTexture);
}
