#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "skybox.h"


MainApp::MainApp()
{
	mUnlitTechnique = NULL;
	GameCamera = NULL;
	mPlanetMesh = NULL;

	mDirectionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	mDirectionalLight.AmbientIntensity = .2f;
	mDirectionalLight.DiffuseIntensity = 1.0f;
	mDirectionalLight.Direction = glm::vec3(0.5f, -0.5f, 0.5f);
	mPersProjInfo.FOV = 60.0f;
	mPersProjInfo.Height = WINDOW_HEIGHT;
	mPersProjInfo.Width = WINDOW_WIDTH;
	mPersProjInfo.zNear = 1.f;
	mPersProjInfo.zFar = 1000.0f;
}

MainApp::~MainApp()
{
	delete GameCamera;
	delete mPlanetMesh;
	delete mUnlitTechnique;

}

bool MainApp::Init()
{
	GameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	mUnlitTechnique = new UnlitTechnique();
	if (!mUnlitTechnique->Init())
		return false;

	mPlanetMesh = new BasicMesh();
	if (!mPlanetMesh->LoadMesh("content/planet.fbx"))
		return false;

	mSkybox = new Skybox(GameCamera, mPersProjInfo);
	if (!mSkybox->Init(
		"content/skybox/right.jpg",
		"content/skybox/left.jpg",
		"content/skybox/top.jpg",
		"content/skybox/bottom.jpg",
		"content/skybox/back.jpg",
		"content/skybox/front.jpg"))
	{
		return false;
	}
	return true;
}

void MainApp::Run()
{
	glClearColor(0.1f, 0.1f, 0.5f, 0.0f);
	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	GLFWBackendRun(this);
}

void MainApp::RenderScene_callback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Some Calculations every frame
	CalcFPS();
	Pipeline p;
	p.SetCamera(GameCamera->GetPos(), GameCamera->GetForward(), GameCamera->GetUp());
	p.SetPerspectiveProj(mPersProjInfo);
	glm::mat4 VP = p.GetVPTrans();
	mUnlitTechnique->Enable();
	p.Scale(glm::vec3(4.0f));
	p.WorldPos(0.0f, -5.0f, 0.0f);
	mUnlitTechnique->SetMVP(VP * p.GetModelTrans());
	mPlanetMesh->Render(mUnlitTechnique);


	// Always render the skybox last
	mSkybox->Render();
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
	GameCamera->OnKeyboard(key, (float)mDeltaTime / 1000);
}

void MainApp::PassiveMouse_callback(float x, float y)
{
	GameCamera->OnMouse(x, y);
}