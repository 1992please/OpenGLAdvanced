#pragma once
#include <fbxsdk.h>
struct MeshInfo
{
	char name[25];
	FbxVector4* vertexes;
};
class FbxFileReader
{
public:
	FbxFileReader(const char*);
	~FbxFileReader();

private:

	FbxManager* sdkManager;
	FbxScene* scene;

	bool LoadScene(const char* pFilename);
	const FbxNode* GetRootNode() const;
	const char * GetRootNodeName() const;

	int numTabs;
	void PrintTabs() const;
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) const;
	void PrintAttribute(FbxNodeAttribute* pAttribute) const;
	void PrintNode(FbxNode* pNode);
};

