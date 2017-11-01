#include "FbxUtil.h"
namespace
{
	const int TRIANGLE_VERTEX_COUNT = 3;
	// Four floats for every position.
	const int VERTEX_STRIDE = 3;
	// Three floats for every normal.
	const int NORMAL_STRIDE = 3;
	// Two floats for every UV.
	const int UV_STRIDE = 2;

	void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
	{
		//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
		pManager = FbxManager::Create();
		if (!pManager)
		{
			fprintf(stderr, "Error: Unable to create FBX Manager!\n");
			exit(1);
		}
		else fprintf(stdout, "Autodesk FBX SDK version %s\n", pManager->GetVersion());

		//Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
		pManager->SetIOSettings(ios);

		//Load plugins from the executable directory (optional)
		FbxString lPath = FbxGetApplicationDirectory();
		pManager->LoadPluginsDirectory(lPath.Buffer());

		//Create an FBX scene. This object holds most objects imported/exported from/to files.
		pScene = FbxScene::Create(pManager, "My Scene");
		if (!pScene)
		{
			fprintf(stderr, "Error: Unable to create FBX scene!\n");
			exit(1);
		}
	}

	void LoadCacheTextures(FbxScene* pScene, const char* pFileName)
	{
		// Load the textures into GPU, only for file texture now
		const int lTextureCount = pScene->GetTextureCount();
		for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
		{
			FbxTexture* lTexture = pScene->GetTexture(lTextureIndex);
			FbxFileTexture* lFileTexture = FbxCast<FbxFileTexture>(lTexture);
			if (lFileTexture && !lFileTexture->GetUserDataPtr())
			{
				// Try to load the texture from absolute path
				const FbxString lFileName = lFileTexture->GetFileName();
				FbxString lResolvedFileName = lFileName;


				const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFileName);
				const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);


				bool bExist = FbxFileUtils::Exist(lResolvedFileName);
				if (!bExist)
				{
					// Load texture from relative file name (relative to FBX file)
					lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
					bExist = FbxFileUtils::Exist(lResolvedFileName);
					
				}

				if (!bExist)
				{
					// Load texture from file name only (relative to FBX file)
					const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
					lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
					bExist = FbxFileUtils::Exist(lResolvedFileName);
				}

				if (bExist)
				{
					FbxString* FinalPath = new FbxString();
					*FinalPath = lResolvedFileName;
					lFileTexture->SetUserDataPtr(FinalPath);
				}
				else
				{
					fprintf(stderr, "Failed to load texture file: %s\n", lFileName.Buffer());
				}

			}
		}
	}

	void UnLoadCacheTextures(FbxScene * pScene)
	{
		const int lTextureCount = pScene->GetTextureCount();
		for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
		{
			FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
			FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
			if (lFileTexture && lFileTexture->GetUserDataPtr())
			{
				FbxString* TexturePath = static_cast<FbxString *>(lFileTexture->GetUserDataPtr());
				lFileTexture->SetUserDataPtr(NULL);
				delete TexturePath;
			}
		}
	}

	void DestroySdkObjects(FbxManager* pManager)
	{
		//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
		if (pManager) pManager->Destroy();
	}

	void FillMeshArrayRecursive(FbxNode* pNode, std::vector<FbxNode*>& pMeshArray)
	{
		if (!pNode)
			return;

		FbxNodeAttribute*const lNodeAttribute = pNode->GetNodeAttribute();
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute && lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			pMeshArray.push_back(pNode);
		}

		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			FillMeshArrayRecursive(pNode->GetChild(lChildIndex), pMeshArray);
		}
	}

	void FillMeshArray(FbxNode* pNode, std::vector<FbxNode*>& pMeshArray)
	{
		pMeshArray.clear();
		FillMeshArrayRecursive(pNode, pMeshArray);
	}
}

CachedMaterial::CachedMaterial()
{
}

CachedMaterial::CachedMaterial(const FbxSurfaceMaterial* pMaterial)
{
	const FbxDouble3 lEmissive = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, mEmissive.TexturePath);
	mEmissive.mColor[0] = static_cast<float>(lEmissive[0]);
	mEmissive.mColor[1] = static_cast<float>(lEmissive[1]);
	mEmissive.mColor[2] = static_cast<float>(lEmissive[2]);

	const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, mAmbient.TexturePath);
	mAmbient.mColor[0] = static_cast<float>(lAmbient[0]);
	mAmbient.mColor[1] = static_cast<float>(lAmbient[1]);
	mAmbient.mColor[2] = static_cast<float>(lAmbient[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, mDiffuse.TexturePath);
	mDiffuse.mColor[0] = static_cast<float>(lDiffuse[0]);
	mDiffuse.mColor[1] = static_cast<float>(lDiffuse[1]);
	mDiffuse.mColor[2] = static_cast<float>(lDiffuse[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, mSpecular.TexturePath);
	mSpecular.mColor[0] = static_cast<float>(lSpecular[0]);
	mSpecular.mColor[1] = static_cast<float>(lSpecular[1]);
	mSpecular.mColor[2] = static_cast<float>(lSpecular[2]);

	FbxProperty lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid())
	{
		double lShininess = lShininessProperty.Get<FbxDouble>();
		mShinness = static_cast<float>(lShininess);
	}
}

FbxDouble3 CachedMaterial::GetMaterialProperty(
	const FbxSurfaceMaterial * pMaterial,
	const char * pPropertyName,
	const char * pFactorPropertyName, 
	char*& pTextureName)
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
				pTextureName = static_cast<FbxString *>(lTexture->GetUserDataPtr())->Buffer();
			}
		}
	}

	return lResult;
}

CachedMaterial::~CachedMaterial()
{
}

FbxUtil::FbxUtil(const char * pFileName)
	: mFileName(pFileName), mSdkManager(NULL), mScene(NULL), mImporter(NULL)
{

}

FbxUtil::~FbxUtil()
{
	mMeshNodes.clear();

	DestroySdkObjects(mSdkManager);
}

bool FbxUtil::Init()
{
	if (mFileName == NULL)
	{
		fprintf(stderr, "No file name supplied.");
		return false;
	}

	// Create the FBX SDK manager which is the object allocator for almost 
	// all the classes in the SDK and create the scene.
	InitializeSdkObjects(mSdkManager, mScene);

	if (mSdkManager)
	{
		// Create the importer.
		int lFileFormat = -1;
		mImporter = FbxImporter::Create(mSdkManager, "");
		if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(mFileName, lFileFormat))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
		}

		// Initialize the importer by providing a filename.
		if (mImporter->Initialize(mFileName, lFileFormat) == true)
		{
			fprintf(stdout, "Importing file............\n");
		}
		else
		{
			fprintf(stderr, "Unable to open file\n");
			return false;
		}
	}
	else
	{
		fprintf(stderr, "Unable to create the FBX SDK manager\n");
		return false;
	}
	return true;
}

bool FbxUtil::load()
{
	if (Init())
	{
		if (mImporter->Import(mScene))
		{
			// Convert Axis System to what is used in this example, if needed
			FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
			FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
			if (SceneAxisSystem != OurAxisSystem)
			{
				OurAxisSystem.ConvertScene(mScene);
			}

			// Convert Unit System to what is used in this example, if needed
			FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
			if (SceneSystemUnit.GetScaleFactor() != 1.0)
			{
				//The unit in this example is centimeter.
				FbxSystemUnit::cm.ConvertScene(mScene);
			}

			// Convert mesh, NURBS and patch into triangle mesh
			FbxGeometryConverter lGeomConverter(mSdkManager);
			lGeomConverter.Triangulate(mScene, true);

			// Bake the scene for one frame
			LoadCacheTextures(mScene, mFileName);
			FillMeshArray(mScene->GetRootNode(), mMeshNodes);
			return true;
		}
	}
	return false;
}

void FbxUtil::GetStaticMeshes(std::vector<CachedMesh*>& Meshes)
{
	Meshes.clear();
	const int lMeshCount = mMeshNodes.size();
	Meshes.resize(lMeshCount);
	for (int i = 0; i < lMeshCount; ++i)
	{
		CachedMesh* lMesh = new CachedMesh();
		if (GetStaticMeshInfo(mMeshNodes[i], lMesh))
			Meshes[i] = lMesh;
	}
}

bool FbxUtil::GetStaticMesh(CachedMesh*& pMesh)
{
	if (mMeshNodes.size() >= 1)
	{
		pMesh = new CachedMesh();
		if (GetStaticMeshInfo(mMeshNodes[0], pMesh))
			return true;
		return false;
	}
}

void FbxUtil::DeAlocate(CachedMesh* pMesh)
{
	delete pMesh;
	pMesh = NULL;
}

bool FbxUtil::GetStaticMeshInfo(FbxNode * pNode, CachedMesh* _Mesh)
{
	// Meshes info----------------------------------------------------
	if (!pNode)
		return false;
	FbxMesh* pFbxMesh = pNode->GetMesh();

	if (!pFbxMesh)
		return false;

	_Mesh->mSubMeshesCount = pNode->GetMaterialCount();
	_Mesh->mSubMeshesOffsets = new int[_Mesh->mSubMeshesCount];
	_Mesh->mSubMeshesTriangleCounts = new int[_Mesh->mSubMeshesCount];

	for (int i = 0; i < _Mesh->mSubMeshesCount; i++)
	{
		_Mesh->mSubMeshesOffsets[i] = 0;
		_Mesh->mSubMeshesTriangleCounts[i] = 0;
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

					_Mesh->mSubMeshesTriangleCounts[lMaterialIndex] += 1;
				}

				// Record the offset (how many vertex)
				const int lMaterialCount = _Mesh->mSubMeshesCount;
				int lOffset = 0;
				for (int lIndex = 0; lIndex < _Mesh->mSubMeshesCount; ++lIndex)
				{
					_Mesh->mSubMeshesOffsets[lIndex] = lOffset;
					lOffset += _Mesh->mSubMeshesTriangleCounts[lIndex] * 3;
					// This will be used as counter in the following procedures, reset to zero
					_Mesh->mSubMeshesTriangleCounts[lIndex] = 0;
				}
				FBX_ASSERT(lOffset == lPolygonCount * 3);
			}
		}
	}

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
	unsigned int* lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];

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
	// if it's indexed by vertexes
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
		const int lIndexOffset = _Mesh->mSubMeshesOffsets[lMaterialIndex] + _Mesh->mSubMeshesTriangleCounts[lMaterialIndex] * 3;
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
		_Mesh->mSubMeshesTriangleCounts[lMaterialIndex] += 1;
	}

	_Mesh->mVertices = lVertices;
	_Mesh->mIndices = lIndices;
	_Mesh->mNormals = lNormals;
	_Mesh->mUVs = lUVs;
	_Mesh->mPolygonVertexCount = lPolygonVertexCount;
	_Mesh->mPolygonCount = lPolygonCount;

	// Material info---------------------------------------------------

	const int lMaterialCount = _Mesh->mSubMeshesCount;
	_Mesh->mMaterials = new CachedMaterial*[lMaterialCount];
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial)
		{
			_Mesh->mMaterials[lMaterialIndex] = new CachedMaterial(lMaterial);
		}
	}

	return true;
}
