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
		Result[3][2] = -2.0f * p.zFar * p.zNear / (p.zFar - p.zNear);
		Result[2][3] = -1.0;
		return Result;
	}

	glm::mat4 InitOrthoProjTransform(const OrthoProjInfo& p)
	{
		glm::mat4 Result(0);
		Result[0][0] = 1/ p.r;
		Result[1][1] = 1/ p.t;
		Result[2][2] = -2.0f /(p.f - p.n);
		Result[3][2] = -(p.f + p.n) / (p.f - p.n);
		Result[3][3] = 1.0;
		return Result;
	}

	glm::mat4 InitCameraTransform(const glm::vec3& Forward, const glm::vec3& Up, const glm::vec3 Pos)
	{
		glm::vec3 f = glm::normalize(Forward);
		glm::vec3 u = glm::normalize(Up);
		glm::vec3 s = glm::cross(f, u);

		glm::mat4 Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = -f.x;
		Result[1][2] = -f.y;
		Result[2][2] = -f.z;
		Result[3][0] = -glm::dot(s, Pos);
		Result[3][1] = -glm::dot(u, Pos);
		Result[3][2] = glm::dot(f, Pos);
		return Result;
	}
}

glm::mat4 Pipeline::GetViewMat(const Camera* camera)
{
	return InitCameraTransform(camera->GetForward(), camera->GetUp(), camera->GetPos());
}

glm::mat4 Pipeline::GetModelMat(const Orientation & ori)
{
	glm::mat4 M;
	M = glm::translate(M, ori.mWorldPos);
	M = glm::scale(M, ori.mScale);
	M = glm::rotate(M, ori.mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, ori.mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, ori.mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	return M;
}

glm::mat4 Pipeline::GetProjMat(const PersProjInfo& p)
{
	return InitPersProjTransform(p);
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
	GetProjTrans();
}

void Pipeline::SetCamera(const glm::vec3 & Pos, const glm::vec3 Forward, const glm::vec3 & Up)
{
	mCamera.Pos = Pos;
	mCamera.Forward = Forward;
	mCamera.Up = Up;
	GetViewTrans();
}

void Pipeline::SetCamera(const Camera* camera)
{
	SetCamera(camera->GetPos(), camera->GetForward(), camera->GetUp());
}

void Pipeline::Orient(const Orientation & o)
{
	mScale = o.mScale;
	mWorldPos = o.mWorldPos;
	mRotateInfo = o.mRotation;
}

const glm::mat4 & Pipeline::GetMPTrans()
{
	GetModelTrans();
	mMP = mP * mM;
	return mMP;
}

const glm::mat4 & Pipeline::GetMVTrans()
{
	GetModelTrans();
	GetViewTrans();
	mMV = mV * mM;
	return mMV;
}

const glm::mat4 & Pipeline::GetVPTrans()
{
	GetViewTrans();
	mVP = mP * mV;
	return mVP;
}

const glm::mat4 & Pipeline::GetMVPTrans()
{
	GetModelTrans();
	GetVPTrans();
	mMVP = mVP * mM;
	return mMVP;
}

const glm::mat4 & Pipeline::GetMVOrthoPTrans()
{
	GetModelTrans();
	GetViewTrans();

	glm::mat4 P = InitOrthoProjTransform(mOrthoProjInfo);
	mMVP = P * mV * mM;
	return mMVP;
}

const glm::mat4 & Pipeline::GetModelTrans()
{
	glm::mat4 M;
	M = glm::translate(M, mWorldPos);
	M = glm::scale(M, mScale);
	M = glm::rotate(M, mRotateInfo.x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, mRotateInfo.y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, mRotateInfo.z, glm::vec3(0.0f, 0.0f, 1.0f));
	mM = M;
	return mM;
}

const glm::mat4 & Pipeline::GetViewTrans()
{
	//mV = glm::lookAt(mCamera.Pos, mCamera.Pos + mCamera.Forward, mCamera.Up);
	mV = InitCameraTransform(mCamera.Forward, mCamera.Up, mCamera.Pos);
	return mV;
}

const glm::mat4 & Pipeline::GetProjTrans()
{
	mP = InitPersProjTransform(mPersProjInfo);
	return mP;
}
