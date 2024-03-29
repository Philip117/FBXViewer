#pragma once

#include <fbxsdk.h>
#include <vector>


namespace Fbx_Extraction
{
	std::vector<FbxNode*> GetModelRoots(FbxScene* pScene);
}