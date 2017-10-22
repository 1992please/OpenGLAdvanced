#include "graphics_app.h"
#include <Windows.h>



GraphicsApp::GraphicsApp()
{
	mFps = 0;
	LARGE_INTEGER PerformanceFrequencyResult;
	QueryPerformanceFrequency(&PerformanceFrequencyResult);
	mPerfCounterFreq = PerformanceFrequencyResult.QuadPart;
	mTickCounter = GetTickCounter();
}

void GraphicsApp::CalcFPS()
{
	const __int64 CurrentTickCounter = GetTickCounter();
	const __int64 ElapsedTicks = CurrentTickCounter - mTickCounter;
	mDeltaTime = (unsigned int) (ElapsedTicks * 1000) / mPerfCounterFreq;
	mFps = mPerfCounterFreq / ElapsedTicks;

	mTickCounter = CurrentTickCounter;
}

void GraphicsApp::RenderFPS()
{
}

float GraphicsApp::GetRunningTime()
{
	return 0.0;
}

__int64 GraphicsApp::GetTickCounter()
{
	LARGE_INTEGER lCounter;
	QueryPerformanceCounter(&lCounter);
	return lCounter.QuadPart;
}
