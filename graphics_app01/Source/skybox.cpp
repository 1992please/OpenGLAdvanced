#include "skybox.h"
#include "pipeline.h"
#include "camera.h"
#include "textures/cubemap_texture.h"
#include "basic_mesh.h"
#include "util.h"
#include "techniques/skybox_technique.h"
//#include "techniques/custom_techniques.h"
#include "engine_common.h"
#include "basic_mesh.h"
#include "pipeline.h"


Skybox::Skybox(const Camera* pCamera, const PersProjInfo& p)
{
	mCamera = pCamera;
	mPersProjInfo = &p;
	mSkyboxTechnique = NULL;
	mCubemapTex = NULL;
	mMesh = NULL;
}

Skybox::~Skybox()
{
	SAFE_DELETE(mSkyboxTechnique);
	SAFE_DELETE(mCubemapTex);
	SAFE_DELETE(mMesh);
}

bool Skybox::Init(
	const char* RightFilename,
	const char* LeftFilename,
	const char* TopFilename,
	const char* BottomFilename,
	const char* BackFilename,
	const char* FrontFilename)
{
	mSkyboxTechnique = new SkyboxTechnique();
	if (!mSkyboxTechnique->Init())
	{
		printf("Error initializing the skybox technique\n");
		return false;
	}
	mSkyboxTechnique->Enable();
	mSkyboxTechnique->SetTextureUnit(DIFFUSE_TEXTURE_UNIT_INDEX);

	mCubemapTex = new CubemapTexture(
		RightFilename,
		LeftFilename,
		TopFilename,
		BottomFilename,
		BackFilename,
		FrontFilename);

	if(!mCubemapTex->Load())
		return false;

	mMesh = new BasicMesh();
	if(!mMesh->LoadMesh(ShapeType_Cube))
		return false;

	return true;
}

void Skybox::Render()
{
	GLint OldCullFaceMode;
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);


	// remove the translation from the view matrix
	glm::mat4 V =glm::mat4(glm::mat3(Pipeline::GetViewMat(mCamera)));
	glm::mat4 P = Pipeline::GetProjMat(*mPersProjInfo);
	glm::mat4 VP = P * V;
	mSkyboxTechnique->Enable();
	mCubemapTex->Bind(DIFFUSE_TEXTURE_UNIT);
	mSkyboxTechnique->SetVP(VP);
	mMesh->Render(mSkyboxTechnique);

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}

