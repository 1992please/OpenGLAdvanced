#ifndef LIGHTS_COMMON_H
#define	LIGHTS_COMMON_H
#include <glm\glm.hpp>
#include <string>
class BaseLight
{
public:
	std::string Name;
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.1f;
		DiffuseIntensity = 0.0f;
	}
	// TODO add ant tweak bar
};

class DirectionalLight : public BaseLight
{
public:
	glm::vec3 Direction;
	DirectionalLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	// TODO add ant tweak bar
};

struct LightAttenuation
{
	float Constant;
	float Linear;
	float Exp;

	LightAttenuation()
	{
		Constant = 1.0f;
		Linear = 0.0f;
		Exp = 0.0f;
	}
};

class PointLight : public BaseLight
{
public:
	glm::vec3 Position;
	LightAttenuation Attenuation;

	PointLight()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	// TODO add ant tweak bar
};

class SpotLight : public PointLight
{
public:
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
	// TODO add ant tweak bar
};

#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec3(0.0f, 0.0f, 1.0f)

#endif // !LIGHTS_COMMON_H

