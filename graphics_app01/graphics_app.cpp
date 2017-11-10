#include "graphics_app.h"
#include <Windows.h>



GraphicsApp::GraphicsApp()
{
	mFps = 0;
	LARGE_INTEGER PerformanceFrequencyResult;
	QueryPerformanceFrequency(&PerformanceFrequencyResult);
	mPerfCounterFreq = PerformanceFrequencyResult.QuadPart;
	mTickCounterStartApp = mTickCounterLastFrame = GetTickCounter();
}

void GraphicsApp::CalcFPS()
{
	const __int64 CurrentTickCounter = GetTickCounter();
	const __int64 ElapsedTicks = CurrentTickCounter - mTickCounterLastFrame;
	mDeltaTime = (unsigned int) ((ElapsedTicks * 1000) / mPerfCounterFreq);
	mFps = unsigned int(mPerfCounterFreq / ElapsedTicks);

	mTickCounterLastFrame = CurrentTickCounter;
}

void GraphicsApp::RenderFPS()
{
}

__int64 GraphicsApp::GetRunningTime() const
{
	return ((GetTickCounter() - mTickCounterStartApp) * 1000) /  mPerfCounterFreq;
}

__int64 GraphicsApp::GetTickCounter() const 
{
	LARGE_INTEGER lCounter;
	QueryPerformanceCounter(&lCounter);
	return lCounter.QuadPart;
}
