#ifndef TECHNIQUES_H
#define	TECHNIQUES_H
#include "Technique.h"
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



class UnlitTechnique : public Technique
{
public:
	UnlitTechnique() {};
	virtual bool Init();
	void SetMVP(const glm::mat4& MVP);
	void SetTextureUnit(unsigned int TextureUnit);

private:
	GLuint mMVPLocation;
	GLuint mSamplerLocation;
};

class CustomTechnique : public Technique
{
public:
	CustomTechnique() {};
	virtual bool Init();
	void SetMVP(const glm::mat4& MVP);

private:
	GLuint mMVPLocation;
};


#endif // !LIGHTINGTECHNIQUE_H
