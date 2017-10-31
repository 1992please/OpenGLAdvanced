#ifndef FBXUTIL_H
#define FBXUTIL_H
#include <fbxsdk.h>
#include <vector>

struct CachedMaterial
{
	CachedMaterial();
	CachedMaterial(const FbxSurfaceMaterial* pMaterial);
	~CachedMaterial();
	static 	FbxDouble3 GetMaterialProperty(
		const FbxSurfaceMaterial* pMaterial,
		const char * pPropertyName, 
		const char * pFactorPropertyName, 
		char*& pTextureName);

	struct CachedColorChannel
	{
		char* TexturePath;
		float mColor[4];

		CachedColorChannel()
		{
			mColor[0] = 0.0f;
			mColor[1] = 0.0f;
			mColor[2] = 0.0f;
			mColor[3] = 1.0f;
			TexturePath = NULL;
		}
	};

	CachedColorChannel mEmissive;
	CachedColorChannel mAmbient;
	CachedColorChannel mDiffuse;
	CachedColorChannel mSpecular;
	float mShinness;
};

struct CachedMesh
{
	float* mVertices;
	float* mNormals;
	float* mUVs;
	unsigned int* mIndices;
	int mPolygonVertexCount;
	int mPolygonCount;
	int* mSubMeshesOffsets;
	int* mSubMeshesTriangleCounts;
	CachedMaterial** mMaterials;
	int mSubMeshesCount;
	CachedMesh() : mPolygonVertexCount(0), mPolygonCount(0), mSubMeshesCount(0) {}
	~CachedMesh()
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
		if (mMaterials)
		{
			for (int lMaterialIndex = 0; lMaterialIndex < mSubMeshesCount; ++lMaterialIndex)
			{
				if (mMaterials[lMaterialIndex])
					delete mMaterials[lMaterialIndex];
			}
			delete[] mMaterials;
		}
	}
};

class FbxUtil
{
public:
	FbxUtil(const char * pFileName);
	~FbxUtil();

	bool load();
	void GetStaticMeshes(std::vector<CachedMesh*>& Meshes);
	void GetStaticMesh(CachedMesh*& pMesh);
	inline void DeAlocate(CachedMesh* pMesh){delete pMesh;}
private:
	bool Init();
	const char* mFileName;
	FbxManager* mSdkManager;
	FbxScene* mScene;
	FbxImporter* mImporter;
	std::vector<FbxNode*> mMeshNodes;

	static bool GetStaticMeshInfo(FbxNode* pNode, CachedMesh* _Mesh);
};

#endif // !FBXUTIL_H