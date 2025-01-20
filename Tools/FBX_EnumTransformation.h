#pragma once
#include <fbxsdk.h>


namespace Fbx_EnumTransformation
{
	std::string EModeToString(const FbxTime::EMode& mode);

	int EModeToInt(const FbxTime::EMode& mode);

	std::string ENodeTypeToString(const FbxNodeAttribute::EType& type);

	std::string ESkeletonTypeToString(const FbxSkeleton::EType& type);

	std::string EUpVectorToString(const FbxAxisSystem::EUpVector& vector);

	std::string EFrontVectorToString(const FbxAxisSystem::EFrontVector& vector);

	std::string ECoordSystemToString(const FbxAxisSystem::ECoordSystem& system);

	std::string EPivorSetToString(const FbxNode::EPivotSet& pivot);

	std::string EOrderToString(const FbxEuler::EOrder& order);

	std::string EPivotSetToString(const FbxNode::EPivotSet& pivot);

	std::string EPivotStateToString(const FbxNode::EPivotState& pivotState);

	std::string EInheritTypeToString(const FbxTransform::EInheritType& inheritType);
}