#pragma once
#include "GlobalDefinitions.h"
#include <fbxsdk.h>

class Mesh;
class FbxSceneReader
{
public:
	FbxSceneReader(const char * pFileName);
	~FbxSceneReader();

	// Return the FBX scene for more informations.
	const FbxScene * GetScene() const { return mScene; }
	// Load the FBX or COLLADA file into memory.
	bool LoadFile();
	void GetStaticMeshes(FbxArray<Mesh*>& Meshes);
private:
	const char * mFileName;
	FbxManager * mSdkManager;
	FbxScene * mScene;
	FbxImporter * mImporter;

	FbxArray<FbxNode*> mMeshes;
};

