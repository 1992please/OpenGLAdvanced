#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
#include "keys.h"
class Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3 Up);
	bool OnKeyboard(KEY Key);
	void OnMouse(int x, int y);
	void OnRender();
	inline const glm::vec3 GetPos() const
	{
		return mPos;
	}
private:
	void Init();
	void Update();

	glm::vec3 mPos;
	glm::vec3 mForward;
	glm::vec3 mRight;
	glm::vec3 mUp;

	int mWindowWidth;
	int mWindowHeight;

	float mYaw;
	float mPitch;

	glm::i32vec2 mMousePos;
};


#endif // !CAMERA_H



