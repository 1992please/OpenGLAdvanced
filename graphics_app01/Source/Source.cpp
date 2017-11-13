#include "glfw_backend.h"
#include "main_app.h"



int main(int argc, char** argv)
{
	system("color f0");
	GLFWBackendInit(argc, argv);
	if (!GLFWBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Graphics Engine"))
	{
		GLFWBackendTerminate();
		return 1;
	}

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
	//return test();
}
