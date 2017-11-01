#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "glm/ext.hpp"
#include "engine_common.h"

MainApp::MainApp()
{
	GameCamera = NULL;
	Technique = NULL;
	mTexture = NULL;
	m_scale = 0.0f;
	mDirectionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	mDirectionalLight.AmbientIntensity = 0.01f;
	mDirectionalLight.DiffuseIntensity = 0.6f;
	mDirectionalLight.Direction = glm::vec3(1.0f, -1.0, 1.0);
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

	Technique = new BasicLightingTechnique();
	if (!Technique->Init())
	{
		return false;
	}
	Technique->Enable();
	Technique->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	Mesh = new BasicMesh();
	Mesh->LoadMesh("C:/Users/Nader/Desktop/nanosuit.fbx");

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
	m_scale += 0.01f;

	PointLight pl[2];
	pl[0].DiffuseIntensity = 0.25f;
	pl[0].Color = glm::vec3(1.0f, 0.5f, 0.0f);
	pl[0].Position = glm::vec3(3.0f, 1.0f, 10 * (cosf(m_scale) + 1.0f) / 2.0f);
	pl[0].Attenuation.Linear = 0.1f;
	pl[1].DiffuseIntensity = 0.25f;
	pl[1].Color = glm::vec3(0.0f, 0.5f, 1.0f);
	pl[1].Position = glm::vec3(7.0f, 1.0f, 10 * (sinf(m_scale) + 1.0f) / 2.0f);
	pl[1].Attenuation.Linear = 0.1f;

	SpotLight sl;
	sl.DiffuseIntensity = 0.9f;
	sl.Color = glm::vec3(0.0f, 1.0f, 1.0f);
	sl.Position = GameCamera->GetPos();
	sl.Direction = GameCamera->GetForward();
	sl.Attenuation.Linear = 0.1f;
	sl.Cutoff = 10.0f;
	Technique->SetSpotLights(1, &sl);
	Technique->SetPointLights(2, pl);

	Pipeline p;
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.SetCamera(GameCamera->GetPos(), GameCamera->GetForward(), GameCamera->GetUp());
	p.SetPerspectiveProj(mPersProjInfo);

	//glm::rotate(V, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(45.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

	Technique->SetMVP(p.GetMVPTrans());
	Technique->SetWorldMatrix(p.GetModelTrans());
	Technique->SetDirectionalLight(mDirectionalLight);
	Technique->SetEyeWorldPos(GameCamera->GetPos());
	Technique->SetMatSpecularIntensity(0.2f);
	Technique->SetMatSpecularPower(32);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the triangle
	Mesh->Render();
}

void MainApp::Keyboard_callback(KEY key)
{
	switch (key)
	{
		case KEY_ESCAPE:
			GLFWBackendLeaveMainLoop();
			break;

		case KEY_Z:
			mDirectionalLight.AmbientIntensity += 0.0005f * mDeltaTime;
			break;

		case KEY_X:
			mDirectionalLight.AmbientIntensity -= 0.0005f * mDeltaTime;
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