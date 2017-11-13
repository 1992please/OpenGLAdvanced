#include "skybox_technique.h"
#include "util.h"

SkyboxTechnique::SkyboxTechnique()
{

}

bool SkyboxTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/skybox.vs", "shaders/skybox.fs")) {
		return false;
	}

	mMVPLocation = GetUniformLocation("gVP");
	mTextureLocation = GetUniformLocation("gCubemapTexture");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mTextureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	return true;
}


void SkyboxTechnique::SetVP(const glm::mat4& VP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &VP[0][0]);
}

void SkyboxTechnique::SetTextureUnit(unsigned int TextureUnitIndex)
{
	glUniform1i(mTextureLocation, TextureUnitIndex);
}