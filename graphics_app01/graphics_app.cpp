#include "graphics_app.h"
#include <Windows.h>


GraphicsApp::GraphicsApp()
{
	mFrameCount = 0;
	mFps = 0;

	mFrameTime = mStartTime = GetTickCount();
}

void GraphicsApp::CalcFPS()
{
	mFrameCount++;

	unsigned long time = GetTickCount();
	if (time - mFrameTime >= 1000)
	{
		mFrameTime = time;
		mFps = mFrameCount;
		mFrameCount = 0;
	}

}

void GraphicsApp::RenderFPS()
{
}

float GraphicsApp::GetRunningTime()
{
	float lRunningTime = ((float)(GetTickCount() - mStartTime)) / 1000.0f;
	return lRunningTime;
}
