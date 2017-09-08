#include "GameObject.h"
#include <string>


const std::string GameObject::positionStr = "position";

GameObject::GameObject()
{
	position = glm::vec3(0.0, 0.0, 0.0);
	rotation = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
}

void GameObject::SetPosition(glm::vec3 pos)
{
	position = pos;
	UpdateTransformationMatrix();
}

void GameObject::SetRotation(glm::vec3 rot)
{
	rotation = rot;
	UpdateObjectVectors();
}

void GameObject::SetScale(glm::vec3 scal)
{
	scale = scal;
}


GameObject::~GameObject()
{
}



void GameObject::UpdateObjectVectors()
{
	forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward.y = sin(glm::radians(rotation.x));
	forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward = glm::normalize(forward);

	right.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.z));
	right.y = sin(glm::radians(rotation.z));
	right.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.z));
	right = glm::normalize(right);


	up = glm::normalize(glm::cross(right, forward));
}

void GameObject::UpdateTransformationMatrix()
{
	glm::mat4 result;
	result = glm::translate(result, position);
	result = glm::rotate(result, glm::radians(glm::length(rotation)), glm::normalize(rotation));
	result = glm::scale(result, scale);
	transformationMatrix = result;
}
