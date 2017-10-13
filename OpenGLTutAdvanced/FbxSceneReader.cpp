#include "SceneCache.h"
#include "FbxSceneReader.h"

namespace
{
	// Default file of ViewScene example
	const char * SAMPLE_FILENAME = "humanoid.fbx";

	void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
	{
		//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
		pManager = FbxManager::Create();
		if (!pManager)
		{
			FBXSDK_printf("Error: Unable to create FBX Manager!\n");
			exit(1);
		}
		else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

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
			FBXSDK_printf("Error: Unable to create FBX scene!\n");
			exit(1);
		}
	}

	void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
	{
		//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
		if (pManager) pManager->Destroy();
		if (pExitStatus) FBXSDK_printf("Program Success!\n");
	}

	void FillMeshArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pMeshArray)
	{
		if (!pNode)
			return;

		FbxNodeAttribute*const lNodeAttribute = pNode->GetNodeAttribute();
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute && lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			pMeshArray.Add(pNode);
		}

		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			FillMeshArrayRecursive(pNode->GetChild(lChildIndex), pMeshArray);
		}
	}

	void FillMeshArray(FbxNode* pNode, FbxArray<FbxNode*>& pMeshArray)
	{
		pMeshArray.Clear();
		FillMeshArrayRecursive(pNode, pMeshArray);
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

				Texture* lTextureObj = new Texture();
				lTextureObj->LoadTexture(lFileName.Buffer());

				const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFileName);
				const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

				if (!lTextureObj->bLoaded)
				{
					// Load texture from relative file name (relative to FBX file)
					const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
					lTextureObj->LoadTexture(lResolvedFileName);
				}

				if (!lTextureObj->bLoaded)
				{
					// Load texture from file name only (relative to FBX file)
					const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
					const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
					lTextureObj->LoadTexture(lResolvedFileName);
				}

				if (!lTextureObj->bLoaded)
				{
					printf("Failed to load texture file: %s\n", lFileName.Buffer());
					continue;
				}

				if (lTextureObj->bLoaded)
				{
					lFileTexture->SetUserDataPtr(lTextureObj);
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
				Texture * lTextureObj = static_cast<Texture *>(lFileTexture->GetUserDataPtr());
				lFileTexture->SetUserDataPtr(NULL);
				delete lTextureObj;
			}
		}
	}

}

FbxSceneReader::FbxSceneReader(const char * pFileName)
	: mFileName(pFileName), mSdkManager(NULL), mScene(NULL), mImporter(NULL)
{
	if (mFileName == NULL)
		mFileName = SAMPLE_FILENAME;


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
			printf("Importing file............\n");
		}
		else
		{
			printf("Unable to open file\n");
		}
	}
	else
	{
		printf("Unable to create the FBX SDK manager\n");
	}
}


FbxSceneReader::~FbxSceneReader()
{
	//delete mDrawText;

	// Unload the cache and free the memory
	if (mScene)
	{
		UnLoadCacheTextures(mScene);
	}

	// Delete the FBX SDK manager. All the objects that have been allocated 
	// using the FBX SDK manager and that haven't been explicitly destroyed 
	// are automatically destroyed at the same time.
	DestroySdkObjects(mSdkManager, true);
}

bool FbxSceneReader::LoadFile()
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
		FillMeshArray(mScene->GetRootNode(), mMeshes);
	}
	return false;
}

void FbxSceneReader::GetStaticMeshes(FbxArray<Mesh*>& Meshes)
{
	Meshes.Clear();
	const int lMeshCount = mMeshes.GetCount();
	Meshes.Resize(lMeshCount);
	for (int i = 0; i < lMeshCount; ++i)
	{
		Meshes[i] = new Mesh(mMeshes[i]);
	}
}

