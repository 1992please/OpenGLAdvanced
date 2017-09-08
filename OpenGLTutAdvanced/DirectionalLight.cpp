#include "DirectionalLight.h"

const std::string DirectionalLight::directionStr = "direction";

DirectionalLight::DirectionalLight()
{
	direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lightStr = "dirLight";
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetShaderUniforms(Shader shader)
{
	Light::SetShaderUniforms(shader);
	shader.setVec3(lightStr + "." + directionStr, direction);
}

void DirectionalLight::SetShaderUniforms(Shader shader, GLuint index)
{
	Light::SetShaderUniforms(shader, index);
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + directionStr, direction);
}
