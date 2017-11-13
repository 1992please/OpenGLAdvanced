#ifndef BASIC_MESH_H
#define BASIC_MESH_H
#include <glm\glm.hpp>
#include <vector>
#include "types.h"

class BasicTechnique;

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

enum ShapeType
{
	ShapeType_Cube,
	ShapeType_Quad,
	ShapeType_Sphere
};

class BasicMesh
{
public:
	BasicMesh();
	~BasicMesh();

	bool LoadMesh(const char* Filename, bool bWithMaterials = true);
	bool LoadMesh(ShapeType Type);
	// use this one wisely( maybe when the camera is static)
	bool InitInstancedStatic(uint NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);
	void Render(BasicTechnique* Technique);
	void RenderDynamic(BasicTechnique* Technique, uint NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);
	void RenderStatic(BasicTechnique* Technique, uint NumInstances);
protected:
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