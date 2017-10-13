#include "camera.h"

const static float STEP_SCALE = 1.0f;
const static int MARGIN = 10;
const static float ROTATION_SCALE = .05f;
const static glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);

Camera::Camera(int WindowWidth, int WindowHeight)
{
	mWindowWidth = WindowWidth;
	mWindowHeight = WindowHeight;
	mPos = glm::vec3(0.0f, 0.0f, 0.0f);
	mForward = glm::vec3(0.0f, 0.0f, 1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::vec3(1.0f, 0.0f, 0.0f);
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

bool Camera::OnKeyboard(KEY Key)
{
	bool lOut = false;
	switch (Key)
	{
		case KEY_UP:
		case KEY_w:
		{
			mPos += (mForward * STEP_SCALE);
			lOut = true;
		}
		break;
		case KEY_DOWN:
		case KEY_s:
		{
			mPos -= (mForward * STEP_SCALE);
			lOut = true;
		}
		break;
		case KEY_RIGHT:
		case KEY_d:
		{
			mPos += mRight * STEP_SCALE;
			lOut = true;

		}
		break;
		case KEY_LEFT:
		case KEY_a:
		{
			mPos -= mRight * STEP_SCALE;
			lOut = true;
		}
		break;
		case KEY_e:
		case KEY_PAGE_UP:
		{
			mPos.y += STEP_SCALE;
			lOut = true;
		}
		break;
		case KEY_q:
		case KEY_PAGE_DOWN:
		{
			mPos.z -= STEP_SCALE;
			lOut = true;
		}
		break;

		default:
			break;
	}
	return lOut;
}

void Camera::OnMouse(int x, int y)
{
	const int DeltaX = x - mMousePos.x;
	const int DeltaY = y - mMousePos.y;

	mMousePos.x = x;
	mMousePos.y = y;

	mYaw += (float)DeltaX * ROTATION_SCALE;
	mPitch += (float)DeltaY * ROTATION_SCALE;

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
		if (HTarget.x >= 0.0f)
		{
			mYaw = 360.0f - glm::degrees(asin(HTarget.z));
		}
		else
		{
			mYaw = 180.0f + glm::degrees(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x > 0.0f)
		{
			mYaw = glm::degrees(asin(-HTarget.z));
		}
		else
		{
			mYaw = 180 - glm::degrees(asin(-HTarget.z));
		}
	}

	mPitch = -glm::degrees(asin(mForward.y));

	mMousePos.x = mWindowWidth / 2;
	mMousePos.y = mWindowHeight / 2;
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
