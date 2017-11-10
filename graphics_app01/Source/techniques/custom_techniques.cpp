#include "util.h"
#include "custom_techniques.h"
#include "../engine_common.h"

LightingTechnique::LightingTechnique()
{
}

bool LightingTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/lighting.vs", "shaders/lighting.fs")) {
		return false;
	}

	mMVPLocation = GetUniformLocation("gMVP");
	mWorldMatrixLocation = GetUniformLocation("gWorld");
	mSamplerLocation = GetUniformLocation("gSampler");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mWorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		mSamplerLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void LightingTechnique::SetMVP(const glm::mat4& MVP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &MVP[0][0]);
}


void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(mSamplerLocation, TextureUnit);
}


void LightingTechnique::SetWorldMatrix(const glm::mat4& Worldmat)
{
	glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &Worldmat[0][0]);
}

void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = glm::normalize(Light.Direction);
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightingTechnique::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechnique::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

bool CustomTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/blending.vs", "shaders/blending.fs")) {
		return false;
	}

	mMVPLocation = GetUniformLocation("MVP");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void CustomTechnique::SetMVP(const glm::mat4 & MVP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &MVP[0][0]);
}

void CustomTechnique::SetMaterial(const Material* mat)
{

}

bool UnlitTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/unlit.vs", "shaders/unlit.fs")) {
		return false;
	}

	mMVPLocation = GetUniformLocation("MVP");
	mSamplerLocation = GetUniformLocation("gSampler");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mSamplerLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void UnlitTechnique::SetMVP(const glm::mat4 & MVP)
{
	glUniformMatrix4fv(mMVPLocation, 1, GL_FALSE, &MVP[0][0]);
}

void UnlitTechnique::SetMaterial(const Material* mat)
{
	glUniform1i(mSamplerLocation, DIFFUSE_TEXTURE_UNIT_INDEX);
}

bool InstancedUnlitTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader("shaders/unlit_instanced.vs", "shaders/unlit_instanced.fs")) {
		return false;
	}

	mSamplerLocation = GetUniformLocation("gSampler");

	if (mMVPLocation == INVALID_UNIFORM_LOCATION ||
		mSamplerLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void InstancedUnlitTechnique::SetMaterial(const Material* mat)
{
	glUniform1i(mSamplerLocation, DIFFUSE_TEXTURE_UNIT_INDEX);
}
