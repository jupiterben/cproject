
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <map>

using json = nlohmann::json;

#include <fbxsdk.h>

#define SAFE_DESTROY(p) \
	if (p)              \
		p->Destroy();   \
	p = nullptr;

/**
* Return a string-based representation based on the attribute type.
*/
std::string GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown:
		return "unidentified";
	case FbxNodeAttribute::eNull:
		return "null";
	case FbxNodeAttribute::eMarker:
		return "marker";
	case FbxNodeAttribute::eSkeleton:
		return "skeleton";
	case FbxNodeAttribute::eMesh:
		return "mesh";
	case FbxNodeAttribute::eNurbs:
		return "nurbs";
	case FbxNodeAttribute::ePatch:
		return "patch";
	case FbxNodeAttribute::eCamera:
		return "camera";
	case FbxNodeAttribute::eCameraStereo:
		return "stereo";
	case FbxNodeAttribute::eCameraSwitcher:
		return "camera switcher";
	case FbxNodeAttribute::eLight:
		return "light";
	case FbxNodeAttribute::eOpticalReference:
		return "optical reference";
	case FbxNodeAttribute::eOpticalMarker:
		return "marker";
	case FbxNodeAttribute::eNurbsCurve:
		return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface:
		return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary:
		return "boundary";
	case FbxNodeAttribute::eNurbsSurface:
		return "nurbs surface";
	case FbxNodeAttribute::eShape:
		return "shape";
	case FbxNodeAttribute::eLODGroup:
		return "lodgroup";
	case FbxNodeAttribute::eSubDiv:
		return "subdiv";
	default:
		return "unknown";
	}
}

class FBXContext
{
protected:
	FbxManager *lSdkManager;
	FbxScene *lScene;

public:
	FBXContext()
	{
		lSdkManager = FbxManager::Create();
		lScene = FbxScene::Create(lSdkManager, "myScene");
	}
	~FBXContext()
	{
		SAFE_DESTROY(lScene);
		SAFE_DESTROY(lSdkManager)
	}

public:
	void load(const char *lFilename)
	{
		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter *lImporter = FbxImporter::Create(lSdkManager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
		{
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		}
		else
		{
			// Import the contents of the file into the scene.
			lImporter->Import(lScene);
		}
		SAFE_DESTROY(lImporter)
	}

	json toJSON()
	{
		json sceneObj(json::value_t::object);
		sceneObj["root"] = nodeToJSON(lScene->GetRootNode());
		return sceneObj;
	}

	json nodeToJSON(FbxNode *pNode)
	{
		json obj(json::value_t::object);

		const char *nodeName = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();

		obj["name"] = nodeName;
		obj["translation"] = {translation[0], translation[1], translation[2]};
		obj["rotation"] = {rotation[0], rotation[1], rotation[2]};
		obj["scaling"] = {scaling[0], scaling[1], scaling[2]};

		std::vector<json> childNodes;
		for (int j = 0; j < pNode->GetChildCount(); j++)
			childNodes.push_back(nodeToJSON(pNode->GetChild(j)));
		if (!childNodes.empty())
			obj["childNodes"] = childNodes;

		// Print the node's attributes.
		json attrs(json::value_t::object);
		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		{
			auto pAttribute = pNode->GetNodeAttributeByIndex(i);
			auto typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
			auto attrName = pAttribute->GetName();
			json attr(json::value_t::object);
			attr["type"] = typeName;
			attrs[attrName] = attr;
		}
		if (!attrs.empty())
			obj["attrs"] = attrs;
		return obj;
	}

	void exportToASCII(const char *lFilename)
	{
		// Create an exporter.
		FbxExporter *lExporter = FbxExporter::Create(lSdkManager, "");

		auto ioSettings = lSdkManager->GetIOSettings();
		// Set the FbxIOSettings EXP_FBX_EMBEDDED property to true.
		ioSettings->SetBoolProp(EXP_FBX_EMBEDDED, true);
		// Get the appropriate file format.
		int lFileFormat = lSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX ascii (*.fbx)");
		// Initialize the exporter to embed the scene's media into the exported file.
		bool lExportStatus = lExporter->Initialize(lFilename, lFileFormat, lSdkManager->GetIOSettings());
		if (!lExportStatus)
		{
			printf("Call to FbxExporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		}
		else
		{
		}
		lExporter->Export(lScene);
		SAFE_DESTROY(lExporter);
	}
};
