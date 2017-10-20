#include <stdio.h>
#include <string.h>
#include <iostream>
#include "util.h"
#include "Technique.h"



Technique::Technique()
{
	mShaderProg = 0;
}

Technique::~Technique()
{
	if (mShaderProg != 0)
	{
		glDeleteProgram(mShaderProg);
		mShaderProg = 0;
	}
}

bool Technique::Init()
{
	mShaderProg = glCreateProgram();
	if (mShaderProg == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}

void Technique::Enable()
{
	glUseProgram(mShaderProg);
}

bool Technique::AddShader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrive the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;

	if (!ReadFile(vertexPath, vertexCode)) {
		return false;
	}

	if (!ReadFile(fragmentPath, fragmentCode)) {
		return false;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// vertex shader
	GLuint vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// fragment shader
	GLuint fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// program compilation
	mShaderProg = glCreateProgram();
	glAttachShader(mShaderProg, vertex);
	glAttachShader(mShaderProg, fragment);
	glLinkProgram(mShaderProg);

#ifdef _DEBUG
	// check to see if compilation was successful
	int  success;
	char infoLog[512];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	glGetProgramiv(mShaderProg, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProg, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
#endif // _DEBUG

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}

GLint Technique::GetUniformLocation(const char * pUniformName)
{
	GLuint Location = glGetUniformLocation(mShaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint Technique::GetProgramParam(GLint param)
{
	GLint Out;
	glGetProgramiv(mShaderProg, param, &Out);
	return Out;
}
