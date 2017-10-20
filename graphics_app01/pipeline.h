#ifndef PIPELINE_H
#define PIPELINE_H

#include <glm\glm.hpp>
#include "camera.h"

struct Orientation
{
	glm::vec3 mScale;
	glm::vec3 mRotation;
	glm::vec3 mWorldPos;

	Orientation()
	{
		mScale = glm::vec3(1.0f, 1.0f, 1.0f);
		mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		mWorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

class Pipeline
{
public:
	Pipeline();
	void Scale(float s);
	void Scale(const glm::vec3& scale);
	void Scale(float ScaleX, float ScaleY, float ScaleZ);
	void WorldPos(float x, float y, float z);
	void WorldPos(const glm::vec3& pos);
	void Rotate(float RotateX, float RotateY, float RotateZ);
	void Rotate(const glm::vec3& r);
	void SetPerspectiveProj(const PersProjInfo& p);
	void SetCamera(const glm::vec3& Pos, const glm::vec3 Target, const glm::vec3& Up);
	void SetCamera(const Camera& camera);
	void Orient(const Orientation& o);

	const glm::mat4& GetMPTrans();
	const glm::mat4& GetMVTrans();
	const glm::mat4& GetVPTrans();
	const glm::mat4& GetMVPTrans();
	const glm::mat4& GetMVOrthoPTrans();
	const glm::mat4& GetModelTrans();
	const glm::mat4& GetViewTrans();
	const glm::mat4& GetProjTrans();
private:
	glm::vec3 mScale;
	glm::vec3 mWorldPos;
	glm::vec3 mRotateInfo;

	PersProjInfo mPersProjInfo;
	OrthoProjInfo mOrthoProjInfo;

	struct {
		glm::vec3 Pos;
		glm::vec3 Forward;
		glm::vec3 Up;
	} mCamera;

	glm::mat4 mMVP;
	glm::mat4 mVP;
	glm::mat4 mMP;
	glm::mat4 mMV;
	glm::mat4 mM;
	glm::mat4 mV;
	glm::mat4 mP;
};
#endif // !PIPELINE_H
