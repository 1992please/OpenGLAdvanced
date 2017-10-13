#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <list>
#include <GL/glew.h>

class Technique
{
public:

	Technique();

	virtual ~Technique();

	virtual bool Init();

	void Enable();

protected:

	bool AddShader(const char* vertexPath, const char* fShaderFile);

	GLint GetUniformLocation(const char* pUniformName);

	GLint GetProgramParam(GLint param);

	GLuint mShaderProg;
};

#endif // !TECHNIQUE_H