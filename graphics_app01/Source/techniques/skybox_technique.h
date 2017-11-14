#ifndef SKYBOX_TECHNIQUE_H
#define	SKYBOX_TECHNIQUE_H

#include "basic_technique.h"
#include <glm/glm.hpp>

class SkyboxTechnique : public BasicTechnique
{
public:

	SkyboxTechnique();

	virtual bool Init() override;

	void SetVP(const glm::mat4& VP);
	void SetMaterial(const Material* mat) override;
private:
	GLuint mMVPLocation;
	GLuint mCubeBoxTexture;
};

#endif
