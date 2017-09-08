#pragma once
#include "GameObject.h"
#include "Shader.h"


class Light :
	public GameObject
{
protected:
	std::string lightStr;
	static const std::string ambientStr;
	static const std::string diffuseStr;
	static const std::string specularStr;
public:

	Light();
	~Light();

	virtual void SetShaderUniforms(Shader shader);
	virtual void SetShaderUniforms(Shader shader, GLuint index);

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

