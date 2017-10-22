#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "keys.h"

class ICallbacks
{
public:

	virtual void Keyboard_callback(KEY key) {};
	virtual void PassiveMouse_callback(float x, float y) {};
	virtual void RenderScene_callback() {};
	virtual void Mouse_callback(MOUSE button, KEY_STATE state, int x, int y) {};
};

#endif