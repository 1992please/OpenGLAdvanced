//#include "FbxUtil.h"
#include "basic_mesh.h"
#include "FbxUtil.h"
#include "util.h"
#include "material.h"
#include <GL/glew.h>
#include "Texture.h"
#include "engine_common.h"

const int VERTEX_STRIDE = 3;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

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
			}

			// Generate and populate the buffers with vertex attributes and the indices
			const int lPolygonVertexCount = lCachedMesh->mPolygonVertexCount;
			const int lPolygonCount = lCachedMesh->mPolygonCount;
			float * const lVertices = lCachedMesh->mVertices;
			float * const lNormals = lCachedMesh->mNormals;
			float * const lUVs = lCachedMesh->mUVs;
			GLuint * const lIndices = lCachedMesh->mIndices;

			glBindBuffer(GL_ARRAY_BUFFER, mBuffers[POS_VB]);
			glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * VERTEX_STRIDE * sizeof(float), lVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glVertexAttribPointer(POSITION_LOCATION, VERTEX_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			if (lNormals)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mBuffers[NORMAL_VB]);
				glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * NORMAL_STRIDE * sizeof(float), lNormals, GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMAL_LOCATION);
				glVertexAttribPointer(NORMAL_LOCATION, NORMAL_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			}

			if (lUVs)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mBuffers[TEXCOORD_VB]);
				glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * UV_STRIDE * sizeof(float), lUVs, GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEX_COORD_LOCATION);
				glVertexAttribPointer(TEX_COORD_LOCATION, UV_STRIDE, GL_FLOAT, GL_FALSE, 0, 0);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[INDEX_BUFFER]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, lPolygonCount * 3 * sizeof(unsigned int), lIndices, GL_STATIC_DRAW);
			Ret = (glGetError() == GL_NO_ERROR);
		}
		FbxUtil::DeAlocate(lCachedMesh);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}

void BasicMesh::Render()
{
	glBindVertexArray(mVAO);

	for (uint i = 0; i < mMeshEntries.size(); i++)
	{
		mMaterials[i]->mDiffuse.mTexture->Bind(COLOR_TEXTURE_UNIT);
		glDrawElements(
			GL_TRIANGLES,
			mMeshEntries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * mMeshEntries[i].BaseIndex));
	}

	// Make sure the VAO is not changed from the outside 
	glBindVertexArray(0);
}

void BasicMesh::Render(uint NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[MVP_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[WORLD_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

	glBindVertexArray(mVAO);

	for (uint i = 0; mMeshEntries.size(); i++)
	{
		if (mMaterials[i]->mDiffuse.mTexture)
			mMaterials[i]->mDiffuse.mTexture->Bind(COLOR_TEXTURE_UNIT);


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
