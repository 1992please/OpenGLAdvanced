#include "reflective_technique.h"
#include "util.h"
#include "engine_common.h"

bool ReflectiveTechnique::Init()
{
	if (!Technique::Init())
		return false;

	if(!AddShader("shaders/reflective_shader.vs", "shaders/reflective_shader.fs"))
		return false;

	mCamerPos = GetUniformLocation("gCameraPos");
	mCubeMap = GetUniformLocation("gCubeMap");
	mMVP = GetUniformLocation("gMVP");
	mModel = GetUniformLocation("gModel");

	if (mCamerPos == INVALID_UNIFORM_LOCATION ||
		mCubeMap == INVALID_UNIFORM_LOCATION ||
		mModel == INVALID_UNIFORM_LOCATION ||
		mMVP == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}
	return true;
}

void ReflectiveTechnique::SetCamerPos(const glm::vec3& Pos)
{
	glUniform3f(mCamerPos, Pos.x, Pos.y, Pos.z);
}

void ReflectiveTechnique::SetMVP(const glm::mat4& MVP)
{
	glUniformMatrix4fv(mMVP, 1, GL_FALSE, &MVP[0][0]);
}

void ReflectiveTechnique::SetModel(const glm::mat4& MVP)
{
	glUniformMatrix4fv(mModel, 1, GL_FALSE, &MVP[0][0]);
}

void ReflectiveTechnique::SetMaterial(const Material* mat)
{
	glUniform1i(mCubeMap, DIFFUSE_TEXTURE_UNIT_INDEX);
}
