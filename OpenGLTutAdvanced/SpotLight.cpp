#include "SpotLight.h"

const std::string SpotLight::constantStr = "constant";
const std::string SpotLight::linearStr = "linear";
const std::string SpotLight::quadraticStr = "quadratic";
const std::string SpotLight::directionStr = "direction";
const std::string SpotLight::innerCutOffStr = "innerCutOff";
const std::string SpotLight::outerCutOffStr = "outerCutOff";


SpotLight::SpotLight()
{
	direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	lightStr = "spotLight";
	innerCutOff = glm::cos(glm::radians(12.5f));
	outerCutOff = glm::cos(glm::radians(17.5f));
}


SpotLight::~SpotLight()
{
}

void SpotLight::SetShaderUniforms(Shader shader)
{
	Light::SetShaderUniforms(shader);

	shader.setFloat(lightStr + "." + constantStr, constant);
	shader.setFloat(lightStr + "." + linearStr, linear);
	shader.setFloat(lightStr + "." + innerCutOffStr, innerCutOff);
	shader.setFloat(lightStr + "." + outerCutOffStr, outerCutOff);
	shader.setFloat(lightStr + "." + quadraticStr, quadratic);
	shader.setVec3(lightStr + "." + positionStr, GetPosition());
	shader.setVec3(lightStr + "." + directionStr, direction);
}

void SpotLight::SetShaderUniforms(Shader shader, GLuint index)
{
	Light::SetShaderUniforms(shader, index);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + constantStr, constant);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + linearStr, linear);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + innerCutOffStr, innerCutOff);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + outerCutOffStr, outerCutOff);
	shader.setFloat(lightStr + "[" + std::to_string(index) + "]" + "." + quadraticStr, quadratic);
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + positionStr, GetPosition());
	shader.setVec3(lightStr + "[" + std::to_string(index) + "]" + "." + directionStr, direction);

}