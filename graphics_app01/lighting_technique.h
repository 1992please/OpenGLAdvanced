#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H
#include "Technique.h"
#include "lights_common.h"

class LightingTechnique : public Technique
{
public:
	LightingTechnique();
	virtual bool Init();
	void SetMVP(const glm::mat4& MVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);

private:
	GLuint mMVPLocation;
	GLuint mSamplerLocation;
	GLuint mDirLightColorLocation;
	GLuint mDirLightAmbientIntensityLocation;
};


#endif // !LIGHTINGTECHNIQUE_H
