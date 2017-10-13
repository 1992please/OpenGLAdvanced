#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "keys.h"

class ICallbacks
{
public:

	virtual void Keyboard_callback(KEY key, KEY_STATE keyState = KEY_STATE_PRESS) {};
	virtual void PassiveMouse_callback(int x, int y) {};
	virtual void RenderScene_callback() {};
	virtual void Mouse_callback(MOUSE button, KEY_STATE state, int x, int y) {};
};

#endif