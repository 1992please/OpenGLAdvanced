#pragma once
#include "basic_technique.h"
#include <glm/glm.hpp>
class ReflectiveTechnique :
	public BasicTechnique
{
public:
	bool Init() override;
	void SetCamerPos(const glm::vec3& Pos);
	void SetMVP(const glm::mat4& MVP);
	void SetModel(const glm::mat4& Model);
	void SetMaterial(const Material* mat) override;
private:
	GLuint mCamerPos;
	GLuint mCubeMap;
	GLuint mMVP;
	GLuint mModel;
};

