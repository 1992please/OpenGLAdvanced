//#include "FbxUtil.h"
#include "basic_mesh.h"
#include "FbxUtil.h"
#include "util.h"
#include "material.h"
#include <GL/glew.h>
#include "Texture.h"
#include "engine_common.h"
#include "techniques/basic_technique.h"

const int VERTEX_STRIDE = 3;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define MVP_LOCATION 3
#define  WORLD_LOCATION 7

static void ShowVertices(const CachedMesh* lMesh)
{
	// Generate and populate the buffers with vertex attributes and the indices
	const int lPolygonVertexCount = lMesh->mPolygonVertexCount;
	const int lPolygonCount = lMesh->mPolygonCount;
	float * const lVertices = lMesh->mVertices;
	float * const lNormals = lMesh->mNormals;
	float * const lUVs = lMesh->mUVs;
	GLuint * const lIndices = lMesh->mIndices;
	for (int i = 0; i < lPolygonVertexCount; i++)
	{
		const int index = i * 3;
		const int uvIndex = i * 2;
		printf("%f, %f, %f, %f, %f, %f, %f, %f\n",
			lVertices[index], lVertices[index + 1], lVertices[index + 2],
			lNormals[index], lNormals[index + 1], lNormals[index + 2],
			lUVs[uvIndex], lUVs[uvIndex + 1]);
	}
}

BasicMesh::BasicMesh()
{
	mVAO = 0;
	memset(mBuffers, 0, sizeof(mBuffers));
}

BasicMesh::~BasicMesh()
{
	Clear();
}

void BasicMesh::Clear()
{
	for (uint i = 0; i < mMaterials.size(); i++)
	{
		SAFE_DELETE(mMaterials[i]);
	}

	if (mBuffers[0] != 0)
	{
		glDeleteBuffers(VB_COUNT, mBuffers);
	}

	if (mVAO != 0)
	{
		glDeleteVertexArrays(1, &mVAO);
		mVAO = 0;
	}
}

bool BasicMesh::LoadMesh(const char* Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();
	// Create the VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(VB_COUNT, mBuffers);



	FbxUtil file(Filename);
	bool Ret = false;
	if (file.load())
	{
		CachedMesh* lCachedMesh;
		if (file.GetStaticMesh(lCachedMesh))
		{
			// Init Meshes................................
			mMeshEntries.resize(lCachedMesh->mSubMeshesCount);
			for (uint i = 0; i < mMeshEntries.size(); i++)
			{
				mMeshEntries[i].NumIndices = lCachedMesh->mSubMeshesTriangleCounts[i] * 3;
				mMeshEntries[i].BaseIndex = lCachedMesh->mSubMeshesOffsets[i];
			}

			// Init Materials................................
			mMaterials.resize(lCachedMesh->mSubMeshesCount);
			for (uint i = 0; i < mMaterials.size(); i++)
			{
				mMaterials[i] = new Material();
				mMaterials[i]->mDiffuse.LoadTexture(lCachedMesh->mMaterials[i]->mDiffuse.TexturePath);
				mMaterials[i]->mDiffuse.LoadColor(lCachedMesh->mMaterials[i]->mDiffuse.mColor);
				mMaterials[i]->mSpecular.LoadTexture(lCachedMesh->mMaterials[i]->mSpecular.TexturePath);
				mMaterials[i]->mSpecular.LoadColor(lCachedMesh->mMaterials[i]->mSpecular.mColor);
				mMaterials[i]->mShininess = lCachedMesh->mMaterials[i]->mShinness;
			}

			// Generate and populate the buffers with vertex attributes and the indices
			const int lPolygonVertexCount = lCachedMesh->mPolygonVertexCount;
			const int lPolygonCount = lCachedMesh->mPolygonCount;
			float * const lVertices = lCachedMesh->mVertices;
			float * const lNormals = lCachedMesh->mNormals;
			float * const lUVs = lCachedMesh->mUVs;
			GLuint * const lIndices = lCachedMesh->mIndices;

			//ShowVertices(lCachedMesh);
			// positions
			glBindBuffer(GL_ARRAY_BUFFER, mBuffers[POS_VB]);
			glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * VERTEX_STRIDE * sizeof(float), lVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glVertexAttribPointer(POSITION_LOCATION, VERTEX_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			// normals
			if (lNormals)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mBuffers[NORMAL_VB]);
				glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * NORMAL_STRIDE * sizeof(float), lNormals, GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMAL_LOCATION);
				glVertexAttribPointer(NORMAL_LOCATION, NORMAL_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			}
			// UVs
			if (lUVs)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mBuffers[TEXCOORD_VB]);
				glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * UV_STRIDE * sizeof(float), lUVs, GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEX_COORD_LOCATION);
				glVertexAttribPointer(TEX_COORD_LOCATION, UV_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			}
			// indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[INDEX_BUFFER]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, lPolygonCount * 3 * sizeof(unsigned int), lIndices, GL_STATIC_DRAW);

			// MVP matrices
			glBindBuffer(GL_ARRAY_BUFFER, mBuffers[MVP_MAT_VB]);
			for (unsigned int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(MVP_LOCATION + i);
				glVertexAttribPointer(MVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(MVP_LOCATION + i, 1);
			}
			// World matrices
			glBindBuffer(GL_ARRAY_BUFFER, mBuffers[WORLD_MAT_VB]);
			for (unsigned int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(WORLD_LOCATION + i);
				glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(WORLD_LOCATION + i, 1);
			}

			Ret = (glGetError() == GL_NO_ERROR);
		}
		FbxUtil::DeAlocate(lCachedMesh);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}

bool BasicMesh::LoadMesh(ShapeType Type)
{
	switch (Type)
	{
		case ShapeType_Cube:
			return LoadMesh("content/basic_shapes/cube.fbx");
		case ShapeType_Quad:
			return LoadMesh("content/basic_shapes/quad.fbx");
		case ShapeType_Sphere:
			return LoadMesh("content/basic_shapes/sphere.fbx");
		default:
			return false;
	}
}

bool BasicMesh::InitInstancedStatic(uint NumInstances, const glm::mat4* MVPMats, const glm::mat4* WorldMats)
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[MVP_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, MVPMats, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[WORLD_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_STATIC_DRAW);
	glBindVertexArray(0);

	return (glGetError() == GL_NO_ERROR);
}

void BasicMesh::Render(BasicTechnique* Technique)
{
	glBindVertexArray(mVAO);
	Technique->Enable();
	for (uint i = 0; i < mMeshEntries.size(); i++)
	{
		mMaterials[i]->mDiffuse.mTexture->Bind(DIFFUSE_TEXTURE_UNIT);
		mMaterials[i]->mSpecular.mTexture->Bind(SPECULAR_TEXTURE_UNIT);

		//mMaterials[i]->mSpecular.mColor = glm::vec3(1);
		Technique->SetMaterial(mMaterials[i]);
		glDrawElements(
			GL_TRIANGLES,
			mMeshEntries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * mMeshEntries[i].BaseIndex));
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void BasicMesh::RenderDynamic(BasicTechnique* Technique, uint NumInstances, const glm::mat4* MVPMats, const glm::mat4* WorldMats)
{
	glBindVertexArray(mVAO);
	if (MVPMats)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[MVP_MAT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, MVPMats, GL_DYNAMIC_DRAW);
	}

	if (WorldMats)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[WORLD_MAT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);
	}


	Technique->Enable();
	for (uint i = 0; i < mMeshEntries.size(); i++)
	{
		mMaterials[i]->mDiffuse.mTexture->Bind(DIFFUSE_TEXTURE_UNIT);
		mMaterials[i]->mSpecular.mTexture->Bind(SPECULAR_TEXTURE_UNIT);

		glDrawElementsInstanced(
			GL_TRIANGLES,
			mMeshEntries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * mMeshEntries[i].BaseIndex),
			NumInstances);
	}
	// Make sure the VAO is not changed from the outside 
	glBindVertexArray(0);
}

void BasicMesh::RenderStatic(BasicTechnique* Technique, uint NumInstances)
{
	Technique->Enable();
	for (uint i = 0; i < mMeshEntries.size(); i++)
	{
		mMaterials[i]->mDiffuse.mTexture->Bind(DIFFUSE_TEXTURE_UNIT);
		mMaterials[i]->mSpecular.mTexture->Bind(SPECULAR_TEXTURE_UNIT);

		glDrawElementsInstanced(
			GL_TRIANGLES,
			mMeshEntries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * mMeshEntries[i].BaseIndex),
			NumInstances);
	}
	// Make sure the VAO is not changed from the outside 
	glBindVertexArray(0);
}
