#pragma once
#include <string>
#include "GlobalDefinitions.h"

class GameObject 
{
protected:
	static const std::string positionStr;
public:
	GameObject();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scal);
	
	inline glm::vec3 GetPosition() const { return position; }
	inline glm::vec3 GetRotation() const { return rotation; }
	inline glm::vec3 GetScale() const { return scale; }
	inline glm::mat4 GetTransformationMatrix() const { return transformationMatrix;	};

	~GameObject();

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 transformationMatrix;

	void UpdateObjectVectors();
	void UpdateTransformationMatrix();
};

