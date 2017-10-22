#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H


class GraphicsApp
{
protected:
	GraphicsApp();

	void CalcFPS();

	void RenderFPS();

	float GetRunningTime();

	__int32 mFps;
	unsigned int mDeltaTime;
private:
	__int64 mTickCounter;
	__int64 mPerfCounterFreq;
	__int64 GetTickCounter();
};


#endif