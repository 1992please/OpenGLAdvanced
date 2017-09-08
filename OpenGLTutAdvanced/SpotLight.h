#pragma once
#include "Light.h"
class SpotLight :
	public Light
{
protected:
	static const std::string constantStr;
	static const std::string linearStr;
	static const std::string quadraticStr;
	static const std::string directionStr;
	static const std::string innerCutOffStr;
	static const std::string outerCutOffStr;
public:
	SpotLight();
	~SpotLight();

public:
	float constant;
	float linear;
	float quadratic;
	glm::vec3 direction;
	float innerCutOff;
	float outerCutOff;


	virtual void SetShaderUniforms(Shader shader) override;
	virtual void SetShaderUniforms(Shader shader, GLuint index) override;
};

