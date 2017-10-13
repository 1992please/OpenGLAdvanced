#include "glfw_backend.h"
#include "camera.h"
#include "MainApp.h"


MainApp::MainApp()
{

}

MainApp::~MainApp()
{

}

bool MainApp::Init()
{
	return true;
}

void MainApp::Run()
{
	GLFWBackendRun(this);
}

void MainApp::RenderScene_callback()
{
}

void MainApp::Keyboard_callback(KEY key, KEY_STATE keyState)
{
}
