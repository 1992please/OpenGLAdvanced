#ifndef SKYBOX_H
#define SKYBOX_H

struct PersProjInfo;
class CubemapTexture;
class BasicMesh;
class Camera;

class Skybox
{
public:
	Skybox(const Camera* pCamera, const PersProjInfo& p);
	~Skybox();

	bool Init(
		const char* RightFilename,
		const char* LeftFilename,
		const char* TopFilename,
		const char* BottomFilename,
		const char* BackFilename,
		const char* FrontFilename);

	void Render();
	inline CubemapTexture* GetCubeMapTex() const { return mCubemapTex; }
private:
	class SkyboxTechnique* mSkyboxTechnique;
	const Camera* mCamera;
	CubemapTexture* mCubemapTex;
	const PersProjInfo* mPersProjInfo;
	BasicMesh* mMesh;
};

#endif