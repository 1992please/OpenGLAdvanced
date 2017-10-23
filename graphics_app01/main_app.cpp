#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "glm/ext.hpp"
MainApp::MainApp()
{
	GameCamera = NULL;
	Technique = NULL;
	mTexture = NULL;
	m_scale = 0.0f;
	mDirectionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	mDirectionalLight.AmbientIntensity = 0.5f;

	mPersProjInfo.FOV = 60.0f;
	mPersProjInfo.Height = WINDOW_HEIGHT;
	mPersProjInfo.Width = WINDOW_WIDTH;
	mPersProjInfo.zNear = .1f;
	mPersProjInfo.zFar = 100.0f;
}

MainApp::~MainApp()
{
	delete Technique;
	delete GameCamera;
	delete mTexture;
}

bool MainApp::Init()
{
	GameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	CreateVertexBuffer();

	Technique = new LightingTechnique();
	if (!Technique->Init())
	{
		return false;
	}
	Technique->Enable();
	Technique->SetTextureUnit(0);

	mTexture = new Texture(GL_TEXTURE_2D);
	mTexture->Load("content/test.png");

	return true;
}

void MainApp::Run()
{
	GLFWBackendRun(this);
}

void MainApp::RenderScene_callback()
{
	CalcFPS();
	//printf("Time: %d\n", mFps);

	//GameCamera->OnRender();

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//m_scale += 0.1f;

	Pipeline p;
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.SetCamera(GameCamera->GetPos(), GameCamera->GetForward(), GameCamera->GetUp());
	p.SetPerspectiveProj(mPersProjInfo);

	//glm::rotate(V, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(45.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	Technique->SetMVP(p.GetMVPTrans());
	Technique->SetDirectionalLight(mDirectionalLight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the triangle
	mTexture->Bind(0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void MainApp::Keyboard_callback(KEY key)
{
	switch (key)
	{
		case KEY_ESCAPE:
			GLFWBackendLeaveMainLoop();
			break;

		case KEY_Z:
			mDirectionalLight.AmbientIntensity += 0.05f;
			break;

		case KEY_X:
			mDirectionalLight.AmbientIntensity -= 0.05f;
			break;
		default:
			break;
	}
	GameCamera->OnKeyboard(key, (float)mDeltaTime/1000);
}

void MainApp::PassiveMouse_callback(float x, float y)
{
	GameCamera->OnMouse(x, y);
}

void MainApp::CreateVertexBuffer()
{
	Vertex vertices[] = { Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),  glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(0.0f, 1.0f, 0.0f),      glm::vec2(0.5f, 1.0f)) };

	unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		1, 2, 0 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}