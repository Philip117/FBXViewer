#include "FBX_EnumTransformation.h"


namespace Fbx_EnumTransformation
{
	std::string EModeToString(const FbxTime::EMode& mode)
	{
		typedef FbxTime::EMode EMode;
		switch (mode)
		{
		case EMode::eDefaultMode:
			return std::string("eDefaultMode - eFrames120");
		case EMode::eFrames100:
			return std::string("eFrames100");
		case EMode::eFrames60:
			return std::string("eFrames60");
		case EMode::eFrames50:
			return std::string("eFrames50");
		case EMode::eFrames48:
			return std::string("eFrames48");
		case EMode::eFrames30:
			return std::string("eFrames30");
		case EMode::eFrames30Drop:
			return std::string("eFrames30Drop");
		case EMode::eNTSCDropFrame:
			return std::string("eNTSCDropFrame");
		case EMode::eNTSCFullFrame:
			return std::string("eNTSCFullFrame");
		case EMode::ePAL:
			return std::string("ePAL");
		case EMode::eFrames24:
			return std::string("eFrames24");
		case EMode::eFrames1000:
			return std::string("eFrames1000");
		case EMode::eFilmFullFrame:
			return std::string("eFilmFullFrame");
		case EMode::eCustom:
			return std::string("eCustom");
		case EMode::eFrames96:
			return std::string("eFrames96");
		case EMode::eFrames72:
			return std::string("eFrames72");
		case EMode::eFrames59dot94:
			return std::string("eFrames59dot94");
		case EMode::eModesCount:
			return std::string("eModesCount");
		default:
			return std::string("Error");
		}
	}

	int EModeToInt(const FbxTime::EMode& mode)
	{
		typedef FbxTime::EMode EMode;
		switch (mode)
		{
		case EMode::eDefaultMode:
			return 120;
		case EMode::eFrames100:
			return 100;
		case EMode::eFrames60:
			return 60;
		case EMode::eFrames50:
			return 50;
		case EMode::eFrames48:
			return 48;
		case EMode::eFrames30:
			return 30;
		case EMode::eFrames30Drop:
			return 0;
		case EMode::eNTSCDropFrame:
			return 0;
		case EMode::eNTSCFullFrame:
			return 0;
		case EMode::ePAL:
			return 0;
		case EMode::eFrames24:
			return 24;
		case EMode::eFrames1000:
			return 1000;
		case EMode::eFilmFullFrame:
			return 0;
		case EMode::eCustom:
			return 0;
		case EMode::eFrames96:
			return 96;
		case EMode::eFrames72:
			return 72;
		case EMode::eFrames59dot94:
			return 94;
		case EMode::eModesCount:
			return 0;
		default:
			return -1;
		}
	}

	std::string ENodeTypeToString(const FbxNodeAttribute::EType& type)
	{
		typedef FbxNodeAttribute::EType EType;
		switch (type)
		{
		case EType::eUnknown:
			return std::string("eUnknown");
		case EType::eNull:
			return std::string("eNull");
		case EType::eMarker:
			return std::string("eMarker");
		case EType::eSkeleton:
			return std::string("eSkeleton");
		case EType::eMesh:
			return std::string("eMesh");
		case EType::eNurbs:
			return std::string("eNurbs");
		case EType::ePatch:
			return std::string("ePatch");
		case EType::eCamera:
			return std::string("eCamera");
		case EType::eCameraStereo:
			return std::string("eCameraStereo");
		case EType::eCameraSwitcher:
			return std::string("eCameraSwitcher");
		case EType::eLight:
			return std::string("eLight");
		case EType::eOpticalReference:
			return std::string("eOpticalReference");
		case EType::eOpticalMarker:
			return std::string("eOpticalMarker");
		case EType::eNurbsCurve:
			return std::string("eNurbsCurve");
		case EType::eTrimNurbsSurface:
			return std::string("eTrimNurbsSurface");
		case EType::eBoundary:
			return std::string("eBoundary");
		case EType::eNurbsSurface:
			return std::string("eNurbsSurface");
		case EType::eShape:
			return std::string("eShape");
		case EType::eLODGroup:
			return std::string("eLODGroup");
		case EType::eSubDiv:
			return std::string("eSubDiv");
		case EType::eCachedEffect:
			return std::string("eCachedEffect");
		case EType::eLine:
			return std::string("eLine");
		default:
			return std::string("Error");
		}
	}

	std::string ESkeletonTypeToString(const FbxSkeleton::EType& type)
	{
		typedef FbxSkeleton::EType EType;
		switch (type)
		{
		case EType::eRoot:
			return std::string("eRoot");
		case EType::eLimb:
			return std::string("eLimb");
		case EType::eLimbNode:
			return std::string("eLimbNode");
		case EType::eEffector:
			return std::string("eEffector");
		default:
			return std::string("Error");
		}
	}

	std::string EUpVectorToString(const FbxAxisSystem::EUpVector& vector)
	{
		typedef FbxAxisSystem::EUpVector EUpVector;
		if (vector == EUpVector::eXAxis)
		{
			return std::string("eXAxis");
		}
		else if (vector == EUpVector::eYAxis)
		{
			return std::string("eYAxis");
		}
		else if (vector == EUpVector::eZAxis)
		{
			return std::string("eZAxis");
		}
		else
		{
			return std::string("Error");
		}
	}

	std::string EFrontVectorToString(const FbxAxisSystem::EFrontVector& vector)
	{
		typedef FbxAxisSystem::EFrontVector EFrontVector;
		if (vector == EFrontVector::eParityEven)
			return std::string("eParityEven");
		else if (vector == EFrontVector::eParityOdd)
			return std::string("eParityOdd");
		else
			return std::string("Error");
	}

	std::string ECoordSystemToString(const FbxAxisSystem::ECoordSystem& system)
	{
		typedef FbxAxisSystem::ECoordSystem ECoordSystem;
		if (system == ECoordSystem::eRightHanded)
			return std::string("eRightHanded");
		else if (system == ECoordSystem::eLeftHanded)
			return std::string("eLeftHanded");
		else
			return std::string("Error");
	}

	std::string EPivorSetToString(const FbxNode::EPivotSet& pivot)
	{
		typedef FbxNode::EPivotSet EPivotSet;
		if (pivot == EPivotSet::eSourcePivot)
			return std::string("eSourcePivot");
		else if (pivot == EPivotSet::eDestinationPivot)
			return std::string("eDestinationPivot");
		else
			return std::string("Error");
	}

	std::string EOrderToString(const FbxEuler::EOrder& order)
	{
		typedef FbxEuler::EOrder EOrder;
		if (order == EOrder::eOrderXYZ)
			return std::string("eOrderXYZ");
		else if (order == EOrder::eOrderXZY)
			return std::string("eOrderXZY");
		else if (order == EOrder::eOrderYZX)
			return std::string("eOrderYZX");
		else if (order == EOrder::eOrderYXZ)
			return std::string("eOrderYXZ");
		else if (order == EOrder::eOrderZXY)
			return std::string("eOrderZXY");
		else if (order == EOrder::eOrderZYX)
			return std::string("eOrderZYX");
		else
			return std::string("Error");
	}
}