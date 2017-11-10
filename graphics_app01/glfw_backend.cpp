#include "util.h"
#include <Windows.h>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "glfw_backend.h"

static ICallbacks* s_pCallbacks = NULL;

static GLFWwindow* s_pWindow = NULL;

static KEY GLFWKeyToOGLDEVKey(uint Key)
{
	if (Key >= GLFW_KEY_SPACE && Key <= GLFW_KEY_RIGHT_BRACKET) {
		return (KEY)Key;
	}

	switch (Key) {
		case GLFW_KEY_ESCAPE:
			return KEY_ESCAPE;
		case GLFW_KEY_ENTER:
			return KEY_ENTER;
		case GLFW_KEY_TAB:
			return KEY_TAB;
		case GLFW_KEY_BACKSPACE:
			return KEY_BACKSPACE;
		case GLFW_KEY_INSERT:
			return KEY_INSERT;
		case GLFW_KEY_DELETE:
			return KEY_DELETE;
		case GLFW_KEY_RIGHT:
			return KEY_RIGHT;
		case GLFW_KEY_LEFT:
			return KEY_LEFT;
		case GLFW_KEY_DOWN:
			return KEY_DOWN;
		case GLFW_KEY_UP:
			return KEY_UP;
		case GLFW_KEY_PAGE_UP:
			return KEY_PAGE_UP;
		case GLFW_KEY_PAGE_DOWN:
			return KEY_PAGE_DOWN;
		case GLFW_KEY_HOME:
			return KEY_HOME;
		case GLFW_KEY_END:
			return KEY_END;
		case GLFW_KEY_F1:
			return KEY_F1;
		case GLFW_KEY_F2:
			return KEY_F2;
		case GLFW_KEY_F3:
			return KEY_F3;
		case GLFW_KEY_F4:
			return KEY_F4;
		case GLFW_KEY_F5:
			return KEY_F5;
		case GLFW_KEY_F6:
			return KEY_F6;
		case GLFW_KEY_F7:
			return KEY_F7;
		case GLFW_KEY_F8:
			return KEY_F8;
		case GLFW_KEY_F9:
			return KEY_F9;
		case GLFW_KEY_F10:
			return KEY_F10;
		case GLFW_KEY_F11:
			return KEY_F11;
		case GLFW_KEY_F12:
			return KEY_F12;
		case GLFW_KEY_LEFT_ALT:
			return KEY_UNDEFINED;
		default:
			ERROR_MESSAGE("Unimplemented OGLDEV key %d\n", Key);
	}

	return KEY_UNDEFINED;
}

static MOUSE GLFWMouseToOGLDEVMouse(uint Button)
{
	switch (Button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			return MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return MOUSE_BUTTON_RIGHT;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return MOUSE_BUTTON_MIDDLE;
		default:
			ERROR_MESSAGE("Unimplemented OGLDEV mouse button %d\n", Button);
	}

	return MOUSE_UNDEFINED;
}

//static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
//{
//	KEY OgldevKey = GLFWKeyToOGLDEVKey(key);
//	KEY_STATE OgldevKeyState = (action == GLFW_PRESS) ? KEY_STATE_PRESS : KEY_STATE_RELEASE;
//	s_pCallbacks->Keyboard_callback(OgldevKey);
//}

void processInput()
{
	if (glfwGetKey(s_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_ESCAPE);
	if (glfwGetKey(s_pWindow, GLFW_KEY_W) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_W);
	if (glfwGetKey(s_pWindow, GLFW_KEY_S) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_S);
	if (glfwGetKey(s_pWindow, GLFW_KEY_E) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_E);
	if (glfwGetKey(s_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_Q);
	if (glfwGetKey(s_pWindow, GLFW_KEY_A) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_A);
	if (glfwGetKey(s_pWindow, GLFW_KEY_D) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_D);	
	if (glfwGetKey(s_pWindow, GLFW_KEY_Z) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_Z);	
	if (glfwGetKey(s_pWindow, GLFW_KEY_X) == GLFW_PRESS)
		s_pCallbacks->Keyboard_callback(KEY_X);
}

static void CursorPosCallback(GLFWwindow* pWindow, double x, double y)
{
	s_pCallbacks->PassiveMouse_callback((float)x, (float)y);
}

static void MouseButtonCallback(GLFWwindow* pWindow, int Button, int Action, int Mode)
{
	MOUSE OgldevMouse = GLFWMouseToOGLDEVMouse(Button);

	KEY_STATE State = (Action == GLFW_PRESS) ? KEY_STATE_PRESS : KEY_STATE_RELEASE;

	double x, y;

	glfwGetCursorPos(pWindow, &x, &y);

	s_pCallbacks->Mouse_callback(OgldevMouse, State, (int)x, (int)y);
}

static void InitCallbacks()
{
	//glfwSetKeyCallback(s_pWindow, KeyCallback);
	glfwSetCursorPosCallback(s_pWindow, CursorPosCallback);
	glfwSetMouseButtonCallback(s_pWindow, MouseButtonCallback);
}

void GLFWErrorCallback(int error, const char* description)
{
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "GLFW error %d - %s", error, description);
	MessageBoxA(NULL, msg, NULL, 0);
	exit(0);
}


void GLFWBackendInit(int argc, char** argv)
{

	glfwSetErrorCallback(GLFWErrorCallback);

	if (glfwInit() != 1) {
		ERROR_MESSAGE0("Error initializing GLFW");
		exit(1);
	}

	int Major, Minor, Rev;

	glfwGetVersion(&Major, &Minor, &Rev);

	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);
}

void GLFWBackendTerminate()
{
	glfwDestroyWindow(s_pWindow);
	glfwTerminate();
}

bool GLFWBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle)
{
	GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;

	// Window Settings
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the Window
	s_pWindow = glfwCreateWindow(Width, Height, pTitle, pMonitor, NULL);

	if (!s_pWindow) {
		ERROR_MESSAGE0("error creating window");
		exit(1);
	}

	glfwMakeContextCurrent(s_pWindow);

	// Must be done after glfw is initialized!
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		ERROR_MESSAGE0((const char*)glewGetErrorString(res));
		exit(1);
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	return (s_pWindow != NULL);
}

void GLFWBackendRun(ICallbacks* pCallbacks)
{
	if (!pCallbacks) {
		ERROR_MESSAGE0("callbacks not specified");
		exit(1);
	}

	// Additional window configurations
	glfwSetWindowPos(s_pWindow, 500, 100);
	glfwSetInputMode(s_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSwapInterval(-1);

	// CallBacks
	s_pCallbacks = pCallbacks;
	InitCallbacks();

	while (!glfwWindowShouldClose(s_pWindow)) 
	{
		processInput();
		s_pCallbacks->RenderScene_callback();
		glfwSwapBuffers(s_pWindow);
		glfwPollEvents();
	}
}

void GLFWBackendSwapBuffers()
{
    // Nothing to do here
}

void GLFWBackendLeaveMainLoop()
{
    glfwSetWindowShouldClose(s_pWindow, 1);
}

void GLFWBackendSetMousePos(uint x, uint y)
{
    glfwSetCursorPos(s_pWindow, (double)x, (double)y);
}