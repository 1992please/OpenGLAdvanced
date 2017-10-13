#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H


class GraphicsApp
{
protected:
	GraphicsApp();

	void CalcFPS();

	void RenderFPS();

	float GetRunningTime();

private:
	unsigned long mFrameTime;
	unsigned long mStartTime;
	int mFrameCount;
	int mFps;
};


#endif