
#include "basic_lighting_technique.h"
#include "util.h"
#include "engine_common.h"
#include "material.h"

BasicLightingTechnique::BasicLightingTechnique()
{
}

bool BasicLightingTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/basic_lighting.vs", "shaders/basic_lighting.fs")) {
		return false;
	}

	mMVPLocation = GetUniformLocation("gWVP");
	mWorldMatrixLocation = GetUniformLocation("gWorld");
	mMaterial.Diffuse.TexLocation = GetUniformLocation("gMaterial.Diffuse.Tex");
	mMaterial.Specular.TexLocation = GetUniformLocation("gMaterial.Specular.Tex");
	mMaterial.Diffuse.Color = GetUniformLocation("gMaterial.Diffuse.Color");
	mMaterial.Specular.Color = GetUniformLocation("gMaterial.Specular.Color");
	mMaterial.Shininess = GetUniformLocation("gMaterial.Shininess");
	mEyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	mDirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	mDirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	mDirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	mDirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	mNumPointLightsLocation = GetUniformLocation("gNumPointLights");
	mNumSpotLightsLocation = GetUniformLocation("gNumSpotLights");

	if (mDirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mWorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
		mEyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		mDirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		mDirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		mDirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		mMaterial.Diffuse.TexLocation == INVALID_UNIFORM_LOCATION ||
		mMaterial.Specular.TexLocation == INVALID_UNIFORM_LOCATION ||
		mMaterial.Diffuse.Color == INVALID_UNIFORM_LOCATION ||
		mMaterial.Specular.Color == INVALID_UNIFORM_LOCATION ||
		mMaterial.Shininess == INVALID_UNIFORM_LOCATION ||
		mNumPointLightsLocation == INVALID_UNIFORM_LOCATION ||
		mNumSpotLightsLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++) {
		char Name[128];
		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		mPointLightsLocation[i].Color = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		mPointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
		mPointLightsLocation[i].Position = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		mPointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		mPointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		mPointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		mPointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (mPointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			mPointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++) {
		char Name[128];
		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		mSpotLightsLocation[i].Color = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		mSpotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		mSpotLightsLocation[i].Position = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		mSpotLightsLocation[i].Direction = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		mSpotLightsLocation[i].Cutoff = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		mSpotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		mSpotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		mSpotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		mSpotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (mSpotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			mSpotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	return true;
}

void BasicLightingTechnique::SetMVP(const glm::mat4& WVP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &WVP[0][0]);
}


void BasicLightingTechnique::SetWorldMatrix(const glm::mat4& WorldInverse)
{
	glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &WorldInverse[0][0]);
}

void BasicLightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(mDirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(mDirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = glm::normalize(Light.Direction);
	glUniform3f(mDirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(mDirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void BasicLightingTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(mEyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void BasicLightingTechnique::SetMaterial(const Material* mat)
{
	glUniform1i(mMaterial.Diffuse.TexLocation, DIFFUSE_TEXTURE_UNIT_INDEX);
	glUniform1i(mMaterial.Specular.TexLocation, SPECULAR_TEXTURE_UNIT_INDEX);
	glUniform1f(mMaterial.Shininess, mat->mShininess);
	glm::vec3 tempColor = mat->mDiffuse.mColor;
	glUniform3f(mMaterial.Diffuse.Color, tempColor.x, tempColor.y, tempColor.z);
	tempColor = mat->mSpecular.mColor;
	glUniform3f(mMaterial.Specular.Color, tempColor.x, tempColor.y, tempColor.z);

}

void BasicLightingTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(mNumPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(mPointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(mPointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(mPointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(mPointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(mPointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(mPointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(mPointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void BasicLightingTechnique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(mNumSpotLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(mSpotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(mSpotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(mSpotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(mSpotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glm::vec3 Direction = glm::normalize(pLights[i].Direction);
		glUniform3f(mSpotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
		glUniform1f(mSpotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].Cutoff)));
		glUniform1f(mSpotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(mSpotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(mSpotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}
