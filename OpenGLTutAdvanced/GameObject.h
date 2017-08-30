#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class GameObject
{
private:
	glm::mat4 TransformationMatrix;
public:
	GameObject();
	~GameObject();
};

