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
	m_scale = 0.0f;
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.5f;

	mPersProjInfo.FOV = 60.0f;
	mPersProjInfo.Height = WINDOW_HEIGHT;
	mPersProjInfo.Width = WINDOW_WIDTH;
	mPersProjInfo.zNear = 1.0f;
	mPersProjInfo.zFar = 100.0f;
}

MainApp::~MainApp()
{
	delete Technique;
	delete GameCamera;
}

bool MainApp::Init()
{
	GameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	CreateVertexBuffer();

	Technique = new CustomTechnique();

	if (!Technique->Init())
	{
		return false;
	}

	Technique->Enable();

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

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the triangle
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainApp::Keyboard_callback(KEY key)
{
	switch (key)
	{
		case KEY_ESCAPE:
			GLFWBackendLeaveMainLoop();
			break;

		case KEY_Z:
			m_directionalLight.AmbientIntensity += 0.05f;
			break;

		case KEY_X:
			m_directionalLight.AmbientIntensity -= 0.05f;
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
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}