#include "FbxFileReader.h"

FbxFileReader::FbxFileReader(const char* filePath)
{
	sdkManager = FbxManager::Create();

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	// Create a scene
	scene = FbxScene::Create(sdkManager, "myScene");

	// Load the scene.
	bool r = LoadScene(filePath);
	if (!r)
	{
		printf("Import Failed");
		return;
	}

	numTabs = 0;

	FbxNode* lRootNode = scene->GetRootNode();
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));
	}
}


FbxFileReader::~FbxFileReader()
{
	if (sdkManager) sdkManager->Destroy();
}


bool FbxFileReader::LoadScene(const char* pFilename)
{
	bool lStatus;

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(sdkManager, "");

	FbxIOSettings * ios = sdkManager->GetIOSettings();

	// Initialize the importer by providing a filename.
	bool lImportStatus = lImporter->Initialize(pFilename, -1, sdkManager->GetIOSettings());

	if (!lImportStatus)
	{
		// Destroy the importer
		lImporter->Destroy();
		return false;
	}

	if (lImporter->IsFBX())
	{
		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		ios->SetBoolProp(IMP_FBX_MATERIAL, true);
		ios->SetBoolProp(IMP_FBX_TEXTURE, true);
		ios->SetBoolProp(IMP_FBX_LINK, true);
		ios->SetBoolProp(IMP_FBX_SHAPE, true);
		ios->SetBoolProp(IMP_FBX_GOBO, true);
		ios->SetBoolProp(IMP_FBX_ANIMATION, true);
		ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene
	lStatus = lImporter->Import(scene);

	// Destroy the importer
	lImporter->Destroy();

	return lStatus;
}

const FbxNode * FbxFileReader::GetRootNode() const
{
	return scene->GetRootNode();
}

const char * FbxFileReader::GetRootNodeName() const
{
	return GetRootNode()->GetName();
}

void FbxFileReader::PrintTabs() const
{
	for (int i = 0; i < numTabs; i++)
		printf("\t");
}

FbxString FbxFileReader::GetAttributeTypeName(FbxNodeAttribute::EType type) const
{
	switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
	}
}

void FbxFileReader::PrintAttribute(FbxNodeAttribute * pAttribute) const
{
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void FbxFileReader::PrintNode(FbxNode * pNode)
{
	PrintTabs();
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the contents of the node.
	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]
	);
	numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j));

	numTabs--;
	PrintTabs();
	printf("</node>\n");
}
