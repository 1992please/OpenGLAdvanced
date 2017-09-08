#include "Light.h"
#include <string>

const std::string Light::ambientStr = "ambient";
const std::string Light::diffuseStr = "diffuse";
const std::string Light::specularStr = "specular";


Light::Light()
{
	ambient = glm::vec3(1);
	diffuse = glm::vec3(1);
	specular = glm::vec3(1);
	lightStr = "light";
}


Light::~Light()
{
}

void Light::SetShaderUniforms(Shader shader)
{
	shader.use();
	shader.setVec3(lightStr + "." + ambientStr, ambient);
	shader.setVec3(lightStr + "." + diffuseStr, diffuse);
	shader.setVec3(lightStr + "." + specularStr, specular);
}

void Light::SetShaderUniforms(Shader shader, GLuint index)
{
	shader.use();
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + ambientStr, ambient);
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + diffuseStr, diffuse);
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + specularStr, specular);
}
