#include <glm\gtc\matrix_transform.hpp>
#include "math_3d.h"
#include "pipeline.h"

namespace
{
	glm::mat4 InitPersProjTransform(const PersProjInfo& p)
	{
		const float tanHalfFovy = tan(ToRadian(p.FOV) / 2.0f);
		const float aspect = p.Width / p.Height;
		glm::mat4 Result(0);
		Result[0][0] = 1.0f / (aspect * tanHalfFovy);
		Result[1][1] = 1.0f / (tanHalfFovy);
		Result[2][2] = -(p.zFar + p.zNear) / (p.zFar - p.zNear);
		Result[2][3] = -2.0f * p.zFar * p.zNear / (p.zFar - p.zNear);
		Result[3][2] = -1.0;
		return Result;
	}
}

Pipeline::Pipeline()
{
	mScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mWorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Pipeline::Scale(float s)
{
	Scale(s, s, s);
}

void Pipeline::Scale(const glm::vec3 & scale)
{
	Scale(scale.x, scale.y, scale.z);
}

void Pipeline::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	mScale.x = ScaleX;
	mScale.y = ScaleY;
	mScale.z = ScaleZ;
}

void Pipeline::WorldPos(float x, float y, float z)
{
	mWorldPos.x = x;
	mWorldPos.y = y;
	mWorldPos.z = z;
}

void Pipeline::WorldPos(const glm::vec3& pos)
{
	mWorldPos = pos;
}

void Pipeline::Rotate(float RotateX, float RotateY, float RotateZ)
{
	mRotateInfo.x = RotateX;
	mRotateInfo.y = RotateY;
	mRotateInfo.z = RotateZ;
}

void Pipeline::Rotate(const glm::vec3 & r)
{
	mRotateInfo = r;
}

void Pipeline::SetPerspectiveProj(const PersProjInfo & p)
{
	mPersProjInfo = p;
}

void Pipeline::SetCamera(const glm::vec3 & Pos, const glm::vec3 Forward, const glm::vec3 & Up)
{
	mCamera.Pos = Pos;
	mCamera.Forward = Forward;
	mCamera.Up = Up;
}

void Pipeline::SetCamera(const Camera & camera)
{
	SetCamera(camera.GetPos(), camera.GetForward(), camera.GetUp());
}

void Pipeline::Orient(const Orientation & o)
{
	mScale = o.mScale;
	mWorldPos = o.mWorldPos;
	mRotateInfo = o.mRotation;
}

const glm::mat4 & Pipeline::GetMPTrans()
{
}

const glm::mat4 & Pipeline::GetMVTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetVPTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetMVPTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetMVOrthoPTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetModelTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetViewTrans()
{
	// TODO: insert return statement here
}

const glm::mat4 & Pipeline::GetProjTrans()
{
	// TODO: insert return statement here
}
