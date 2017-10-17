#pragma once
#include "GlobalDefinitions.h"
#include <fbxsdk.h>

struct FStaticMesh;
struct FMaterial
{
public:
	FMaterial();
	FMaterial(const FbxSurfaceMaterial* pMaterial);
	~FMaterial();
	void UseShader(Shader* pShader);
	bool HasTexture() const { return mDiffuse.mTexture != 0; }
	static 	FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial* pMaterial, const char * pPropertyName, const char * pFactorPropertyName, Texture** pTextureName);
private:
	struct ColorChannel
	{
		Texture* mTexture;
		GLfloat mColor[4];

		ColorChannel()
		{
			mColor[0] = 0.0f;
			mColor[1] = 0.0f;
			mColor[2] = 0.0f;
			mColor[3] = 1.0f;
			mTexture = NULL;
		}
	};

	ColorChannel mEmissive;
	ColorChannel mAmbient;
	ColorChannel mDiffuse;
	ColorChannel mSpecular;
	GLfloat mShinness;
};

class Mesh
{
public:

	/* Mesh Data */
	Mesh(FbxNode* pNode);
	~Mesh();

	// Draw all the faces with specific material with given shading mode.
	void DrawALL(ShadingMode ShadingMode, Shader* pShader);
	void Draw(int pMaterialIndex, ShadingMode pShadingMode) const;
private:
	enum
	{
		VERTEX_VBO,
		NORMAL_VBO,
		UV_VBO,
		INDEX_VBO,
		VBO_COUNT
	};
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}
		int IndexOffset;
		int TriangleCount;
	};

	// For every material, record the offsets in every VBO and triangle counts
	FbxArray<SubMesh*> mSubMeshes;
	FMaterial** mMaterials;
	GLuint mVBOs[VBO_COUNT], mVAO;
	bool mHasNormal;
	bool mHasUV;
	bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

	bool InitializeSubMeshes(const FStaticMesh* lMesh);
};