#include "camera.h"
#include <math.h>

const static glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);

Camera::Camera(int WindowWidth, int WindowHeight)
{
	mWindowWidth = WindowWidth;
	mWindowHeight = WindowHeight;
	mPos = glm::vec3(0.0f, 0.0f, -2.0f);
	mForward = glm::vec3(0.0f, 0.0f, 1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::vec3(1.0f, 0.0f, 0.0f);
	bFirstMouseCB = true;
	Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3 & Pos, const glm::vec3 & Target, const glm::vec3 Up)
{
	mWindowWidth = WindowWidth;
	mWindowHeight = WindowHeight;
	mPos = Pos;
	mForward = glm::normalize(Target);
	mUp = glm::normalize(Up);
	mRight = glm::normalize(glm::cross(mForward, WORLD_UP));
	Init();
}

bool Camera::OnKeyboard(KEY Key, float deltaTime)
{
	float velocity = SPEED * deltaTime;
	bool lOut = false;
	switch (Key)
	{
		case KEY_UP:
		case KEY_W:
		{
			mPos += (mForward * velocity);
			lOut = true;
		}
		break;
		case KEY_DOWN:
		case KEY_S:
		{
			mPos -= (mForward * velocity);
			lOut = true;
		}
		break;
		case KEY_RIGHT:
		case KEY_D:
		{
			mPos += mRight * velocity;
			lOut = true;

		}
		break;
		case KEY_LEFT:
		case KEY_A:
		{
			mPos -= mRight * velocity;
			lOut = true;
		}
		break;
		case KEY_E:
		case KEY_PAGE_UP:
		{
			mPos.y += velocity;
			lOut = true;
		}
		break;
		case KEY_Q:
		case KEY_PAGE_DOWN:
		{
			mPos.y -= velocity;
			lOut = true;
		}
		break;

		default:
			break;
	}
	return lOut;
}

void Camera::OnMouse(float x, float y)
{
	if (bFirstMouseCB)
	{
		mMousePos.x = x;
		mMousePos.y = y;
		bFirstMouseCB = false;
	}
	const float DeltaX = (x - mMousePos.x) * SENSITIVTY;
	const float DeltaY = (mMousePos.y - y) * SENSITIVTY;

	mMousePos.x = x;
	mMousePos.y = y;

	mYaw = std::fmod((mYaw + DeltaX), 360.0f);
	mPitch += DeltaY;

	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;

	Update();
}

void Camera::OnRender()
{
}

void Camera::Init()
{
	glm::vec3 HTarget(mForward.x, 0.0, mForward.z);
	glm::normalize(HTarget);

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x > 0.0f)
		{
			mYaw = glm::degrees(asin(HTarget.z));
		}
		else
		{
			mYaw = 180 - glm::degrees(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			mYaw = 360.0f - glm::degrees(asin(-HTarget.z));
		}
		else
		{
			mYaw = 180.0f + glm::degrees(asin(-HTarget.z));
		}
	}

	mPitch = -glm::degrees(asin(mForward.y));
}

void Camera::Update()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mForward = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mForward, WORLD_UP));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	mUp = glm::normalize(glm::cross(mRight, mForward));
}
