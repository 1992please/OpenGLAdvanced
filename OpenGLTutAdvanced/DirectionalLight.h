#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
protected:
	static const std::string directionStr;
public:
	DirectionalLight();
	~DirectionalLight();

public:
	glm::vec3 direction;

	virtual void SetShaderUniforms(Shader shader) override;
	virtual void SetShaderUniforms(Shader shader, GLuint index) override;
};

