#ifndef BAISC_TECHNIQUE_H
#define BAISC_TECHNIQUE_H
#include "technique.h"

class Material;

class BasicTechnique : public Technique
{
public:
	BasicTechnique();
	~BasicTechnique();
	virtual void SetMaterial(const Material* mat) {};
};

#endif // !BAISC_TECHNIQUE_H



