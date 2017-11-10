#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "glm/ext.hpp"

MainApp::MainApp()
{
	mUnlitTechnique = NULL;
	mInstUnlitTechnique = NULL;
	GameCamera = NULL;
	mPlanetMesh = NULL;
	mRockMesh = NULL;

	mDirectionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	mDirectionalLight.AmbientIntensity = .2f;
	mDirectionalLight.DiffuseIntensity = 1.0f;
	mDirectionalLight.Direction = glm::vec3(0.5f, -0.5f, 0.5f);
	mPersProjInfo.FOV = 60.0f;
	mPersProjInfo.Height = WINDOW_HEIGHT;
	mPersProjInfo.Width = WINDOW_WIDTH;
	mPersProjInfo.zNear = 1.0f;
	mPersProjInfo.zFar = 1000.0f;
	bFirstRender = true;
	mAmount = 200000;
}

MainApp::~MainApp()
{
	delete GameCamera;
	delete mRockMesh;
	delete mUnlitTechnique;
	delete mInstUnlitTechnique;
	delete[] Models;
	delete[] MVP;
}

bool MainApp::Init()
{
	GameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	mUnlitTechnique = new UnlitTechnique();
	if (!mUnlitTechnique->Init())
		return false;

	mInstUnlitTechnique = new InstancedUnlitTechnique();
	if (!mInstUnlitTechnique->Init())
		return false;

	mPlanetMesh = new BasicMesh();
	if (!mPlanetMesh->LoadMesh("content/planet.fbx"))
		return false;

	mRockMesh = new BasicMesh();
	if (!mRockMesh->LoadMesh("content/rock.fbx"))
		return false;

	srand(0);


	Orientations = new Orientation[mAmount];
	float radius = 50.0;
	float offset = 20.f;
	for (uint i = 0; i < mAmount; i++)
	{
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)mAmount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.1f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		Orientations[i].mWorldPos = glm::vec3(x, y, z);

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05f;
		Orientations[i].mScale = glm::vec3(scale);

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = float(rand() % 360);
		Orientations[i].mRotation = glm::vec3(0.0f, 0.0f, rotAngle);
	}
	Models = new glm::mat4[mAmount];
	MVP = new glm::mat4[mAmount];
	return true;
}

void MainApp::Run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	// render the rocks
	p.Scale(glm::vec3(1.0f));
	if (bFirstRender)
	{
		bFirstRender = !bFirstRender;
		for (uint i = 0; i < mAmount; i++)
		{
			p.Orient(Orientations[i]);
			Models[i] = p.GetModelTrans();
		}
	}
	__int64 Time = GetRunningTime();
	for (uint i = 0; i < mAmount; i++)
	{
		p.Orient(Orientations[i]);
		MVP[i] = VP * Models[i];
	}
	printf("1: %I64d, ", GetRunningTime() - Time);
	Time = GetRunningTime();
	mRockMesh->RenderDynamic(mInstUnlitTechnique, mAmount, MVP, NULL);
	printf("2: %I64d, fps: %d\n", GetRunningTime() - Time, mFps);

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