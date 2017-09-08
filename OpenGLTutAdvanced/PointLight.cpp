#include "PointLight.h"


const std::string PointLight::constantStr = "constant";
const std::string PointLight::linearStr = "linear";
const std::string PointLight::quadraticStr = "quadratic";

PointLight::PointLight()
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	lightStr = "pointLights";
}


PointLight::~PointLight()
{
}

void PointLight::SetShaderUniforms(Shader shader)
{
	Light::SetShaderUniforms(shader);
	shader.setFloat(lightStr + "." + constantStr, constant);
	shader.setFloat(lightStr + "." + linearStr, linear);
	shader.setFloat(lightStr + "." + quadraticStr, quadratic);
	shader.setVec3(lightStr + "." + positionStr, GetPosition());
}

void PointLight::SetShaderUniforms(Shader shader, GLuint index)
{
	Light::SetShaderUniforms(shader, index);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + constantStr, constant);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + linearStr, linear);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + quadraticStr, quadratic);
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + positionStr, GetPosition());

}
