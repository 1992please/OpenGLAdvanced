#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H


class GraphicsApp
{
protected:
	GraphicsApp();

	void CalcFPS();

	void RenderFPS();

	__int64 GetRunningTime() const;

	__int32 mFps;
	unsigned int mDeltaTime;
private:
	__int64 mTickCounterLastFrame;
	__int64 mTickCounterStartApp;
	__int64 mPerfCounterFreq;
	__int64 GetTickCounter() const;
};


#endif