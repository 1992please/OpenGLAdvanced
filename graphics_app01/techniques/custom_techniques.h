#ifndef TECHNIQUES_H
#define	TECHNIQUES_H
#include "basic_technique.h"
#include "lights_common.h"

class LightingTechnique : public Technique
{
public:
	LightingTechnique();
	virtual bool Init();
	void SetMVP(const glm::mat4& MVP);
	void SetWorldMatrix(const glm::mat4& WorldInverse);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
private:
	GLuint mMVPLocation;
	GLuint mWorldMatrixLocation;
	GLuint mSamplerLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	} m_dirLightLocation;
};



class UnlitTechnique : public BasicTechnique
{
public:
	UnlitTechnique() {};
	virtual bool Init();
	void SetMVP(const glm::mat4& MVP);
	void SetMaterial(const Material* mat) override;
private:
	GLuint mMVPLocation;
	GLuint mSamplerLocation;
};

class InstancedUnlitTechnique : public BasicTechnique
{
public:
	InstancedUnlitTechnique() {};
	virtual bool Init();
	void SetMaterial(const Material* mat) override;
private:
	GLuint mMVPLocation;
	GLuint mSamplerLocation;
};

class CustomTechnique : public BasicTechnique
{
public:
	CustomTechnique() {};
	bool Init() override;
	void SetMVP(const glm::mat4& MVP);
	void SetMaterial(const Material* mat) override;
private:
	GLuint mMVPLocation;
};


#endif // !LIGHTINGTECHNIQUE_H
