#pragma once
#include "Light.h"
class PointLight :
	public Light
{
protected:
	static const std::string constantStr;
	static const std::string linearStr;
	static const std::string quadraticStr;

public:
	PointLight();
	~PointLight();

public:
	float constant;
	float linear;
	float quadratic;

	virtual void SetShaderUniforms(Shader shader) override;
	virtual void SetShaderUniforms(Shader shader, GLuint index) override;
};

