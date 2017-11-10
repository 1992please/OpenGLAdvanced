#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
#include "keys.h"


const float SPEED = 10.f;
const float SENSITIVTY = 0.1f;

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

struct OrthoProjInfo
{
	float r;
	float t;
	float n;        // z near
	float f;        // z far
};

class Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3 Up);
	bool OnKeyboard(KEY Key, float deltaTime);
	void OnMouse(float x, float y);
	void OnRender();
	inline const glm::vec3& GetPos() const{ return mPos; }
	inline const glm::vec3& GetForward() const { return mForward; }
	inline const glm::vec3& GetUp() const { return mUp; }
	void SetPos(const glm::vec3& Pos);
	// TODO add to ant tool bar
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

	glm::vec2 mMousePos;
	bool bFirstMouseCB;
};


#endif // !CAMERA_H



