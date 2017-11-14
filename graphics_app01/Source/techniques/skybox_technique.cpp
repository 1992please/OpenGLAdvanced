#include "skybox_technique.h"
#include "util.h"
#include "engine_common.h"

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
	mCubeBoxTexture = GetUniformLocation("gCubemapTexture");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mCubeBoxTexture == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	return true;
}


void SkyboxTechnique::SetVP(const glm::mat4& VP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &VP[0][0]);
}

void SkyboxTechnique::SetMaterial(const Material* mat)
{
	glUniform1i(mCubeBoxTexture, DIFFUSE_TEXTURE_UNIT);
}
