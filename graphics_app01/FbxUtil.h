#ifndef FBXUTIL_H
#define FBXUTIL_H

#include <vector>
class fbxsdk::FbxNode;

struct FMaterial
{
public:
	FMaterial();
	FMaterial(const FbxSurfaceMaterial* pMaterial);
	~FMaterial();
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

struct FStaticMesh
{
	float* mVertices;
	float* mNormals;
	float* mUVs;
	GLuint* mIndices;
	int mVertexCount;
	int mPolygonCount;
	int* mSubMeshesOffsets;
	int* mSubMeshesTriangleCounts;
	FMaterial** mMaterials;
	int mSubMeshesCount;
	FStaticMesh() : mVertexCount(0), mPolygonCount(0), mSubMeshesCount(1) {}
	~FStaticMesh()
	{
		if (mVertices)
			delete[] mVertices;
		if (mNormals)
			delete[] mNormals;
		if (mUVs)
			delete[] mUVs;
		if (mIndices)
			delete[] mIndices;
		if (mSubMeshesOffsets)
			delete[] mSubMeshesOffsets;
		if (mSubMeshesTriangleCounts)
			delete[] mSubMeshesTriangleCounts;
	}
};

class FbxUtil
{
public:
	FbxUtil(const char * pFileName);
	~FbxUtil();
	bool load();
	void GetStaticMeshes(std::vector<FStaticMesh*>& Meshes);
	void GetStaticMesh(FStaticMesh* pMesh);

private:
	const char * mFileName;
	FbxManager * mSdkManager;
	FbxScene * mScene;
	FbxImporter * mImporter;
	std::vector<FbxNode*> mMeshNodes;

	static bool GetStaticMeshInfo(FbxNode* pNode, FStaticMesh* _Mesh);
};

#endif // !FBXUTIL_H