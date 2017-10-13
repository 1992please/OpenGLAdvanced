#include "glfw_backend.h"
#include "MainApp.h"


int main(int argc, char** argv)
{
	GLFWBackendInit(argc, argv, true, false);
	if (!GLFWBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Graphics Engine"))
	{
		GLFWBackendTerminate();
		return 1;
	}
	SRANDOM;

	MainApp* pApp = new MainApp();

	if (!pApp->Init())
	{
		delete pApp;
		GLFWBackendTerminate();
		return 1;
	}

	pApp->Run();

	GLFWBackendTerminate();

	return 0;
}