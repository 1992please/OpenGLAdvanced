#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "skybox.h"
#include "techniques\reflective_technique.h"
#include "techniques\refractive_technique.h"
#include "textures\cubemap_texture.h"
#include "engine_common.h"

MainApp::MainApp()
{
	mReflectiveTechnique = NULL;
	mRefractiveTechnique = NULL;
	mCamera = NULL;
	mSphere = NULL;

	mPersProjInfo.FOV = 60.0f;
	mPersProjInfo.Height = WINDOW_HEIGHT;
	mPersProjInfo.Width = WINDOW_WIDTH;
	mPersProjInfo.zNear = 0.1f;
	mPersProjInfo.zFar = 1000.0f;
}

MainApp::~MainApp()
{
	delete mCamera;
	delete mSphere;

	delete mReflectiveTechnique;
	delete mRefractiveTechnique;
}

bool MainApp::Init()
{
	mCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	mReflectiveTechnique = new ReflectiveTechnique();
	if (!mReflectiveTechnique->Init())
		return false;

	mRefractiveTechnique = new RefractiveTechnique();
	if (!mRefractiveTechnique->Init())
		return false;

	mSphere = new BasicMesh();
	if (!mSphere->LoadMesh(ShapeType_Cube))
		return false;

	mSkybox = new Skybox(mCamera, mPersProjInfo);
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
	mSkybox->GetCubeMapTex()->Bind(DIFFUSE_TEXTURE_UNIT);
	// Some Calculations every frame
	CalcFPS();
	glm::mat4 V = Pipeline::GetViewMat(mCamera);
	glm::mat4 P = Pipeline::GetProjMat(mPersProjInfo);
	glm::mat4 VP = P * V;
	glm::mat4 M;
	mRefractiveTechnique->Enable();
	mRefractiveTechnique->SetMVP(VP);
	mRefractiveTechnique->SetModel(M);
	mRefractiveTechnique->SetCamerPos(mCamera->GetPos());
	mSphere->Render(mRefractiveTechnique);

	Orientation ori;
	ori.mWorldPos = glm::vec3(2, 0, 0);
	M = Pipeline::GetModelMat(ori);
	mReflectiveTechnique->Enable();
	mReflectiveTechnique->SetMVP(VP * M);
	mReflectiveTechnique->SetModel(M);
	mReflectiveTechnique->SetCamerPos(mCamera->GetPos());
	mSphere->Render(mReflectiveTechnique);

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
			break;

		case KEY_X:
			break;
		default:
			break;
	}
	mCamera->OnKeyboard(key, (float)mDeltaTime / 1000);
}

void MainApp::PassiveMouse_callback(float x, float y)
{
	mCamera->OnMouse(x, y);
}