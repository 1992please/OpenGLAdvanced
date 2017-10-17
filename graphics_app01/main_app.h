#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "util.h"
#include "graphics_app.h"
#include "callbacks.h"
#include <GL\glew.h>
#include "Texture.h"
#include "lighting_technique.h"
#include "camera.h"

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;

	Vertex() {}

	Vertex(glm::vec3 pos, glm::vec2 tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

class MainApp :public ICallbacks, public GraphicsApp
{
public:
	MainApp();
	~MainApp();
	bool Init();
	void Run();
	void RenderScene_callback() override;
	void Keyboard_callback(KEY key, KEY_STATE keyState = KEY_STATE_PRESS) override;

private:
	void CreateVertexBuffer()
	{
		Vertex Vertices[4] = { Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
			Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),  glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.0f, 1.0f, 0.0f),      glm::vec2(0.5f, 1.0f)) };

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void CreateIndexBuffer()
	{
		unsigned int Indices[] = { 0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			1, 2, 0 };

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	}

	GLuint m_VBO;
	GLuint m_IBO;
	LightingTechnique* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	PersProjInfo m_persProjInfo;
};

#endif // !MAIN_APP_H