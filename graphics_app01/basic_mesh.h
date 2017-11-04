#ifndef BASIC_MESH_H
#define BASIC_MESH_H
#include <glm\glm.hpp>
#include <vector>
#include "types.h"
#include "basic_lighting_technique.h"

struct Vertex
{
	glm::vec3 mPos;
	glm::vec2 mTex;
	glm::vec3 mNormal;

	Vertex() {}
	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
	{
		mPos = pos;
		mTex = tex;
		mNormal = normal;
	}
};

class Material;
struct CachedMesh;

class BasicMesh
{
public:
	BasicMesh();
	~BasicMesh();

	bool LoadMesh(const char* Filename);

	void Render(BasicLightingTechnique* Technique);
	void Render(uint NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);
private:
	void Clear();

	enum
	{
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		INDEX_BUFFER,
		MVP_MAT_VB,
		WORLD_MAT_VB,
		VB_COUNT
	};

	uint mVAO;
	uint mBuffers[VB_COUNT];

	struct BasicMeshEntry {
		BasicMeshEntry()
		{
			NumIndices = 0;
			BaseIndex = 0;
		}
		uint NumIndices;
		uint BaseIndex;
	};
	std::vector<BasicMeshEntry> mMeshEntries;
	std::vector<Material*> mMaterials;
};

#endif // !BASICMESH_H