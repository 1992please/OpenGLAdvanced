#include <vector>
#include "SceneCache.h"

const int TRIANGLE_VERTEX_COUNT = 3;
// Four floats for every position.
const int VERTEX_STRIDE = 3;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

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

static bool GetStaticMeshInfo(FbxNode* pNode, FStaticMesh& _Mesh)
{
	// Meshes info----------------------------------------------------
	if (!pNode)
		return false;
	FbxMesh* pFbxMesh = pNode->GetMesh();

	if (!pFbxMesh)
		return false;

	_Mesh.mSubMeshesCount = pNode->GetMaterialCount();
	_Mesh.mSubMeshesOffsets = new int[_Mesh.mSubMeshesCount];
	_Mesh.mSubMeshesTriangleCounts = new int[_Mesh.mSubMeshesCount];

	for (int i = 0; i < _Mesh.mSubMeshesCount; i++)
	{
		_Mesh.mSubMeshesOffsets[i] = 0;
		_Mesh.mSubMeshesTriangleCounts[i] = 0;
	}

	const int lPolygonCount = pFbxMesh->GetPolygonCount();

	// Count the polygon count of each material
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	if (pFbxMesh->GetElementMaterial())
	{
		lMaterialIndice = &pFbxMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pFbxMesh->GetElementMaterial()->GetMappingMode();

		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
			if (lMaterialIndice->GetCount() == lPolygonCount)
			{
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
				{
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);

					_Mesh.mSubMeshesTriangleCounts[lMaterialIndex] += 1;
				}

				// Record the offset (how many vertex)
				const int lMaterialCount = _Mesh.mSubMeshesCount;
				int lOffset = 0;
				for (int lIndex = 0; lIndex < _Mesh.mSubMeshesCount; ++lIndex)
				{
					_Mesh.mSubMeshesOffsets[lIndex] = lOffset;
					lOffset += _Mesh.mSubMeshesTriangleCounts[lIndex] * 3;
					// This will be used as counter in the following procedures, reset to zero
					_Mesh.mSubMeshesTriangleCounts[lIndex] = 0;
				}
				FBX_ASSERT(lOffset == lPolygonCount * 3);
			}
		}
	}
	// All faces will use the same material.
	//if (_Mesh.SubMeshesCount == 0)
	//{
	//	_Mesh.SubMeshesCount = 1;
	//	mSubMeshes[0] = new SubMesh();
	//}

	// Congregate all the data of a mesh to be cached in VBOs.
	// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
	bool lHasNormal = pFbxMesh->GetElementNormalCount() > 0;
	bool lHasUV = pFbxMesh->GetElementUVCount() > 0;
	bool lAllByControlPoint = true;

	FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
	if (lHasNormal)
	{
		lNormalMappingMode = pFbxMesh->GetElementNormal(0)->GetMappingMode();
		if (lNormalMappingMode == FbxGeometryElement::eNone)
		{
			lHasNormal = false;
		}
		if (lHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
		{
			lAllByControlPoint = false;
		}
	}
	if (lHasUV)
	{
		lUVMappingMode = pFbxMesh->GetElementUV(0)->GetMappingMode();
		if (lUVMappingMode == FbxGeometryElement::eNone)
		{
			lHasUV = false;
		}
		if (lHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
		{
			lAllByControlPoint = false;
		}
	}

	// Allocate the array memory, by control point or by polygon vertex.
	int lPolygonVertexCount = pFbxMesh->GetControlPointsCount();
	if (!lAllByControlPoint)
	{
		lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
	}
	float* lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
	GLuint* lIndices = new GLuint[lPolygonCount * TRIANGLE_VERTEX_COUNT];

	float* lNormals = NULL;
	if (lHasNormal)
	{
		lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
	}
	float* lUVs = NULL;
	FbxStringList lUVNames;
	pFbxMesh->GetUVSetNames(lUVNames);
	const char* lUVName = NULL;
	if (lHasUV && lUVNames.GetCount())
	{
		lUVs = new float[lPolygonVertexCount * UV_STRIDE];
		lUVName = lUVNames[0];
	}

	// Populate the array with vertex attribute, if by control point
	const FbxVector4* lControlPoints = pFbxMesh->GetControlPoints();
	FbxVector4 lCurrentVertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;
	// if it's indiced by vertexes
	if (lAllByControlPoint)
	{
		const FbxGeometryElementNormal * lNormalElement = NULL;
		const FbxGeometryElementUV * lUVElement = NULL;

		// Save the normal
		if (lHasNormal)
		{
			lNormalElement = pFbxMesh->GetElementNormal(0);
		}
		if (lHasUV)
		{
			lUVElement = pFbxMesh->GetElementUV(0);
		}
		for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
		{
			// Save the vertex position.
			lCurrentVertex = lControlPoints[lIndex];
			lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
			lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
			lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);

			// Save the normal.
			if (lHasNormal)
			{
				int lNormalIndex = lIndex;
				if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
				lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
				lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
			}

			// Save the UV.
			if (lHasUV)
			{
				int lUVIndex = lIndex;
				if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
				lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
				lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
			}
		}
	}

	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
	{
		// the material for current face
		int lMaterialIndex = 0;
		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
		}

		// Where should I save the vertex attribute index, according to the material
		const int lIndexOffset = _Mesh.mSubMeshesOffsets[lMaterialIndex] + _Mesh.mSubMeshesTriangleCounts[lMaterialIndex] * 3;
		for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
		{
			const int lControlPointIndex = pFbxMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

			if (lAllByControlPoint)
			{
				lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
			}
			// Populate the array with vertex attribute, if by polygon vertex.
			else
			{
				lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

				lCurrentVertex = lControlPoints[lControlPointIndex];
				lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
				lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
				lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);

				if (lHasNormal)
				{
					pFbxMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
					lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
					lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
					lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
				}

				if (lHasUV)
				{
					bool lUnmappedUV;
					pFbxMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
					lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
					lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
				}
			}
			++lVertexCount;
		}
		_Mesh.mSubMeshesTriangleCounts[lMaterialIndex] += 1;
	}

	_Mesh.mVertices = lVertices;
	_Mesh.mIndices = lIndices;
	_Mesh.mNormals = lNormals;
	_Mesh.mUVs = lUVs;
	_Mesh.mVertexCount = lPolygonVertexCount;
	_Mesh.mPolygonCount = lPolygonCount;

	// Material info---------------------------------------------------

	const int lMaterialCount = _Mesh.mSubMeshesCount;
	_Mesh.mMaterials = new FMaterial*[lMaterialCount];
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial)
		{
			_Mesh.mMaterials[lMaterialIndex] = new FMaterial(lMaterial);
		}
	}

	return true;
}


FMaterial::FMaterial()
{
}

FMaterial::FMaterial(const FbxSurfaceMaterial* pMaterial)
{
	const FbxDouble3 lEmissive = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, &mEmissive.mTexture);
	mEmissive.mColor[0] = static_cast<GLfloat>(lEmissive[0]);
	mEmissive.mColor[1] = static_cast<GLfloat>(lEmissive[1]);
	mEmissive.mColor[2] = static_cast<GLfloat>(lEmissive[2]);

	const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, &mAmbient.mTexture);
	mAmbient.mColor[0] = static_cast<GLfloat>(lAmbient[0]);
	mAmbient.mColor[1] = static_cast<GLfloat>(lAmbient[1]);
	mAmbient.mColor[2] = static_cast<GLfloat>(lAmbient[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, &mDiffuse.mTexture);
	mDiffuse.mColor[0] = static_cast<GLfloat>(lDiffuse[0]);
	mDiffuse.mColor[1] = static_cast<GLfloat>(lDiffuse[1]);
	mDiffuse.mColor[2] = static_cast<GLfloat>(lDiffuse[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, &mSpecular.mTexture);
	mSpecular.mColor[0] = static_cast<GLfloat>(lSpecular[0]);
	mSpecular.mColor[1] = static_cast<GLfloat>(lSpecular[1]);
	mSpecular.mColor[2] = static_cast<GLfloat>(lSpecular[2]);

	FbxProperty lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid())
	{
		double lShininess = lShininessProperty.Get<FbxDouble>();
		mShinness = static_cast<GLfloat>(lShininess);
	}
}


FbxDouble3 FMaterial::GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName, Texture** pTexture)
{
	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if (lProperty.IsValid())
	{
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture && lTexture->GetUserDataPtr())
			{
				*pTexture = static_cast<Texture *>(lTexture->GetUserDataPtr());
			}
		}
	}

	return lResult;
}

FMaterial::~FMaterial()
{
}

void FMaterial::UseShader(Shader * pShader)
{
	if (mDiffuse.mTexture)
		mDiffuse.mTexture->Bind(0);
	if (mSpecular.mTexture)
		mSpecular.mTexture->Bind(1);

	pShader->use();
	pShader->setInt("material.diffuse", 0);
	pShader->setInt("material.specular", 1);
	pShader->setFloat("material.shininess", 32.0f);
}

Mesh::Mesh(FbxNode* pNode) :mAllByControlPoint(true), mHasUV(false), mHasNormal(false)
{
	FStaticMesh lMesh;
	if (GetStaticMeshInfo(pNode, lMesh))
	{
		InitializeSubMeshes(&lMesh);
		mMaterials = lMesh.mMaterials;
	}

}

Mesh::~Mesh()
{

	for (int i = 0; i < mSubMeshes.GetCount(); i++)
	{
		delete mSubMeshes[i];
	}

	for (int i = 0; i < mSubMeshes.GetCount(); i++)
	{
		delete mMaterials[i];
	}

	mSubMeshes.Clear();
	delete[] mMaterials;
}





bool Mesh::InitializeSubMeshes(const FStaticMesh* lMesh)
{
	// just to continue the code
	mSubMeshes.Resize(lMesh->mSubMeshesCount);
	for (int i = 0; i < mSubMeshes.GetCount(); i++)
	{
		mSubMeshes[i] = new SubMesh();
		mSubMeshes[i]->IndexOffset = lMesh->mSubMeshesOffsets[i];
		mSubMeshes[i]->TriangleCount = lMesh->mSubMeshesTriangleCounts[i];
	}
	// Create VBOs
	const int lPolygonVertexCount = lMesh->mVertexCount;
	const int lPolygonCount = lMesh->mPolygonCount;
	float * const lVertices = lMesh->mVertices;
	float * const lNormals = lMesh->mNormals;
	float * const lUVs = lMesh->mUVs;
	GLuint * const lIndices = lMesh->mIndices;

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(VBO_COUNT, mVBOs);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[VERTEX_VBO]);
	glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * VERTEX_STRIDE * sizeof(float), lVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_STRIDE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE * sizeof(float), (void*)0);
	if (lNormals)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBOs[NORMAL_VBO]);
		glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * NORMAL_STRIDE * sizeof(float), lNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, NORMAL_STRIDE, GL_FLOAT, GL_FALSE, NORMAL_STRIDE * sizeof(float), (void*)0);
	}

	if (lUVs)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBOs[UV_VBO]);
		glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * UV_STRIDE * sizeof(float), lUVs, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, UV_STRIDE, GL_FLOAT, GL_FALSE, UV_STRIDE * sizeof(float), (void*)0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOs[INDEX_VBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lPolygonCount * TRIANGLE_VERTEX_COUNT * sizeof(unsigned int), lIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return true;
}

void Mesh::DrawALL(ShadingMode ShadingMode, Shader* pShader)
{
	for (int i = 0; i < mSubMeshes.GetCount(); ++i)
	{
		mMaterials[i]->UseShader(pShader);
		Draw(i, ShadingMode);
	}
}

void Mesh::Draw(int pMaterialIndex, ShadingMode pShadingMode) const
{
	// Where to start.
	GLsizei lOffset = mSubMeshes[pMaterialIndex]->IndexOffset * sizeof(unsigned int);
	if (pShadingMode == SHADING_MODE_SHADED)
	{
		const GLsizei lElementCount = mSubMeshes[pMaterialIndex]->TriangleCount * 3;
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, lElementCount, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(lOffset));
		glBindVertexArray(0);
	}
	else
	{
		glBindVertexArray(mVAO);
		for (int lIndex = 0; lIndex < mSubMeshes[pMaterialIndex]->TriangleCount; ++lIndex)
		{
			// Draw line loop for every triangle.
			glDrawElements(GL_LINE_LOOP, TRIANGLE_VERTEX_COUNT, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(lOffset));
			lOffset += sizeof(unsigned int) * TRIANGLE_VERTEX_COUNT;
		}
		glBindVertexArray(0);
	}
}

//if (!pMesh->GetNode())
//	return false;

//const int lPolygonCount = pMesh->GetPolygonCount();

//// Count the polygon count of each material
//FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
//FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
//if (pMesh->GetElementMaterial())
//{
//	lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
//	lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();

//	if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
//	{
//		FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
//		if (lMaterialIndice->GetCount() == lPolygonCount)
//		{
//			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
//			{
//				const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
//				if (mSubMeshes.GetCount() < lMaterialIndex + 1)
//				{
//					mSubMeshes.Resize(lMaterialIndex + 1);
//				}

//				if (mSubMeshes[lMaterialIndex] == NULL)
//				{
//					mSubMeshes[lMaterialIndex] = new SubMesh;
//				}
//				mSubMeshes[lMaterialIndex]->TriangleCount += 1;
//			}

//			// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
//			// if, in the loop above, we resized the mSubMeshes by more than one slot.
//			for (int i = 0; i < mSubMeshes.GetCount(); i++)
//			{
//				if (mSubMeshes[i] == NULL)
//					mSubMeshes[i] = new SubMesh;
//			}

//			// Record the offset (how many vertex)
//			const int lMaterialCount = mSubMeshes.GetCount();
//			int lOffset = 0;
//			for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
//			{
//				mSubMeshes[lIndex]->IndexOffset = lOffset;
//				lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
//				// This will be used as counter in the following procedures, reset to zero
//				mSubMeshes[lIndex]->TriangleCount = 0;
//			}
//			FBX_ASSERT(lOffset == lPolygonCount * 3);
//		}
//	}
//}
//// All faces will use the same material.
//if (mSubMeshes.GetCount() == 0)
//{
//	mSubMeshes.Resize(1);
//	mSubMeshes[0] = new SubMesh();
//}

//// Congregate all the data of a mesh to be cached in VBOs.
//// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
//mHasNormal = pMesh->GetElementNormalCount() > 0;
//mHasUV = pMesh->GetElementUVCount() > 0;
//FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
//FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
//if (mHasNormal)
//{
//	lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
//	if (lNormalMappingMode == FbxGeometryElement::eNone)
//	{
//		mHasNormal = false;
//	}
//	if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
//	{
//		mAllByControlPoint = false;
//	}
//}
//if (mHasUV)
//{
//	lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
//	if (lUVMappingMode == FbxGeometryElement::eNone)
//	{
//		mHasUV = false;
//	}
//	if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
//	{
//		mAllByControlPoint = false;
//	}
//}

//// Allocate the array memory, by control point or by polygon vertex.
//int lPolygonVertexCount = pMesh->GetControlPointsCount();
//if (!mAllByControlPoint)
//{
//	lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
//}
//float* lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
//GLuint* lIndices = new GLuint[lPolygonCount * TRIANGLE_VERTEX_COUNT];

//float* lNormals = NULL;
//if (mHasNormal)
//{
//	lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
//}
//float* lUVs = NULL;
//FbxStringList lUVNames;
//pMesh->GetUVSetNames(lUVNames);
//const char* lUVName = NULL;
//if (mHasUV && lUVNames.GetCount())
//{
//	lUVs = new float[lPolygonVertexCount * UV_STRIDE];
//	lUVName = lUVNames[0];
//}

//// Populate the array with vertex attribute, if by control point
//const FbxVector4* lControlPoints = pMesh->GetControlPoints();
//FbxVector4 lCurrentVertex;
//FbxVector4 lCurrentNormal;
//FbxVector2 lCurrentUV;
//if (mAllByControlPoint)
//{
//	const FbxGeometryElementNormal * lNormalElement = NULL;
//	const FbxGeometryElementUV * lUVElement = NULL;

//	// Save the normal
//	if (mHasNormal)
//	{
//		lNormalElement = pMesh->GetElementNormal(0);
//	}
//	if (mHasUV)
//	{
//		lUVElement = pMesh->GetElementUV(0);
//	}
//	for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
//	{
//		// Save the vertex position.
//		lCurrentVertex = lControlPoints[lIndex];
//		lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
//		lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
//		lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);

//		// Save the normal.
//		if (mHasNormal)
//		{
//			int lNormalIndex = lIndex;
//			if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
//			{
//				lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
//			}
//			lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
//			lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
//			lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
//			lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
//		}

//		// Save the UV.
//		if (mHasUV)
//		{
//			int lUVIndex = lIndex;
//			if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
//			{
//				lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
//			}
//			lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
//			lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
//			lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
//		}
//	}
//}

//int lVertexCount = 0;
//for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
//{
//	// the material for current face
//	int lMaterialIndex = 0;
//	if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
//	{
//		lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
//	}

//	// Where should I save the vertex attribute index, according to the material
//	const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset + mSubMeshes[lMaterialIndex]->TriangleCount * 3;
//	for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
//	{
//		const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

//		if (mAllByControlPoint)
//		{
//			lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
//		}
//		// Populate the array with vertex attribute, if by polygon vertex.
//		else
//		{
//			lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

//			lCurrentVertex = lControlPoints[lControlPointIndex];
//			lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
//			lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
//			lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);

//			if (mHasNormal)
//			{
//				pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
//				lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
//				lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
//				lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
//			}

//			if (mHasUV)
//			{
//				bool lUnmappedUV;
//				pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
//				lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
//				lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
//			}
//		}
//		++lVertexCount;
//	}
//	mSubMeshes[lMaterialIndex]->TriangleCount += 1;
//}
