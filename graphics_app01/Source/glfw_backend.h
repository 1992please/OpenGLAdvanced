#ifndef GLFW_BACKEND_H
#define GLFW_BACKEND_H
#include "types.h"
#include "callbacks.h"

void GLFWBackendInit(int argc, char** argv);

void GLFWBackendTerminate();

bool GLFWBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void GLFWBackendRun(ICallbacks* pCallbacks);

void GLFWBackendSwapBuffers();

void GLFWBackendLeaveMainLoop();

void GLFWBackendSetMousePos(uint x, uint y);
#endif
