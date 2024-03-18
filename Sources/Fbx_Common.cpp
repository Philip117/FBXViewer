#include <Windows.h>
#include "../Headers/Fbx_Common.h"


namespace Fbx_Common
{
	bool  InitializeManagerAndScene(FbxManager*& pManager, FbxScene*& pScene)
	{
		if (pManager || pScene)
			return false;

		pManager = FbxManager::Create();
		if (!pManager)
			return false;

		if (!pManager->GetIOSettings())
			pManager->SetIOSettings(FbxIOSettings::Create(pManager, IOSROOT));
		//FbxString lPath = FbxGetApplicationDirectory();
		//pManager->LoadPluginsDirectory(lPath);

		pScene = FbxScene::Create(pManager, "VIRDYN Scene");
		if (!pScene)
			return false;

		return true;
	}

	bool  InitializeManager(FbxManager*& pManager)
	{
		if (pManager)
			pManager->Destroy();
		if (!pManager)
			pManager = FbxManager::Create();
		if (!pManager)
			return false;

		if (!pManager->GetIOSettings())
			pManager->SetIOSettings(FbxIOSettings::Create(pManager, IOSROOT));
		//FbxString lPath = FbxGetApplicationDirectory();
		//pManager->LoadPluginsDirectory(lPath);

		return true;
	}

	bool  InitializeScene(FbxManager*& pManager, FbxScene*& pScene)
	{
		if (!pManager)
			return false;

		if (pScene)
			pScene->Destroy();
		pScene = FbxScene::Create(pManager, "VIRDYN Scene");
		if (!pScene)
			return false;

		return true;
	}

	void  DestroyManager(FbxManager*& pManager)
	{
		if (pManager)
			pManager->Destroy();
		pManager = nullptr;
	}

	void  DestroyScene(FbxScene*& pScene)
	{
		if (pScene)
			pScene->Destroy();
		pScene = nullptr;
	}

	void  SetImportBoolProp(FbxManager* const pManager, const bool& material, const bool& texture, const bool& link,
		const bool& shape, const bool& gobo, const bool& animation, const bool& globalSetting)
	{
		FbxIOSettings* lpIoSetting = pManager->GetIOSettings();
		lpIoSetting->SetBoolProp(IMP_FBX_MATERIAL, material);				// 材质
		lpIoSetting->SetBoolProp(IMP_FBX_TEXTURE, texture);					// 纹理
		lpIoSetting->SetBoolProp(IMP_FBX_LINK, link);						// 该参数仅用于导入时
		lpIoSetting->SetBoolProp(IMP_FBX_SHAPE, shape);						// 外形？导入面部表情需要设置为 true
		lpIoSetting->SetBoolProp(IMP_FBX_GOBO, gobo);						// ？
		lpIoSetting->SetBoolProp(IMP_FBX_ANIMATION, animation);				// 动画
		lpIoSetting->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, globalSetting);	// 全局设置
	}

	void  SetExportBoolProp(FbxManager* const pManager, const bool& material, const bool& texture, const bool& embedded,
		const bool& shape, const bool& gobo, const bool& animation, const bool& globalSetting)
	{
		FbxIOSettings* lpIoSetting = pManager->GetIOSettings();
		lpIoSetting->SetBoolProp(EXP_FBX_MATERIAL, material);				// 材质
		lpIoSetting->SetBoolProp(EXP_FBX_TEXTURE, texture);					// 纹理
		lpIoSetting->SetBoolProp(EXP_FBX_EMBEDDED, embedded);				// 该参数仅用于导出时
		lpIoSetting->SetBoolProp(EXP_FBX_SHAPE, shape);						// 外形？导入面部表情需要设置为 true
		lpIoSetting->SetBoolProp(EXP_FBX_GOBO, gobo);						// ？
		lpIoSetting->SetBoolProp(EXP_FBX_ANIMATION, animation);				// 动画
		lpIoSetting->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, globalSetting);	// 全局设置
	}

	bool  LoadFbxFile(FbxManager* pManager, FbxScene* pScene, const std::string& filePath)
	{
		FbxImporter* lpImporter = FbxImporter::Create(pManager, "VIRDYN Importer");

		// 初始化加载器
		if (!lpImporter->Initialize(filePath.c_str(), -1, pManager->GetIOSettings()))	// pFileFormat == -1 表示由 SDK 自行检测文件格式
			//if (!lpImporter->Initialize(lFilePath, -1, pManager->GetIOSettings()))	// pFileFormat == -1 表示由 SDK 自行检测文件格式
			return false;
		// 检测是否为 FBX 文件
		if (!lpImporter->IsFBX())
			return false;
		// 导入到场景
		if (!lpImporter->Import(pScene))
			return false;
		lpImporter->Destroy();
		//delete[] lFilePath;

		return true;
	}

	bool  SetCoordSystem(FbxScene* const pScene, const FbxAxisSystem::EUpVector& vector_up,
		const FbxAxisSystem::EFrontVector& vector_front, const FbxAxisSystem::ECoordSystem& coordSystem)
	{
		if (!pScene)
			return false;

		// 坐标系强制转换
		const FbxAxisSystem lAxisSystem(vector_up, vector_front, coordSystem);
		lAxisSystem.ConvertScene(pScene);	// 强制转换
		return true;
	}

	bool  GetCoordSystem(FbxScene* const pScene, FbxAxisSystem::EUpVector& vector_up, int& sign_up,
		FbxAxisSystem::EFrontVector& vector_front, int& sign_front, FbxAxisSystem::ECoordSystem& coordSystem)
	{
		if (!pScene)
			return false;

		FbxAxisSystem lAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();
		vector_up = lAxisSystem.GetUpVector(sign_up);
		vector_front = lAxisSystem.GetFrontVector(sign_front);
		coordSystem = lAxisSystem.GetCoorSystem();
		return true;
	}

	bool  SetUnit(FbxScene* const pScene, const FbxSystemUnit& unit)
	{
		if (!pScene)
			return false;

		// 长度单位转换
		FbxGlobalSettings& lGlobalSetting = pScene->GetGlobalSettings();
		for (const FbxSystemUnit& u : { lGlobalSetting.GetSystemUnit().mm, lGlobalSetting.GetSystemUnit().dm, lGlobalSetting.GetSystemUnit().cm,lGlobalSetting.GetSystemUnit().m,lGlobalSetting.GetSystemUnit().km,lGlobalSetting.GetSystemUnit().Inch,lGlobalSetting.GetSystemUnit().Foot,lGlobalSetting.GetSystemUnit().Mile, lGlobalSetting.GetSystemUnit().Yard })
		{
			if (u == unit)
			{
				u.ConvertScene(pScene);
				break;
			}
		}

		return true;
	}

	bool  SaveScene(FbxManager* const pManager, FbxDocument* const pScene, const std::string& filePath, int fileFormat)
	{
		if (!pManager)
			return false;
		if (!pScene)
			return false;

		int	lFormatCount = pManager->GetIOPluginRegistry()->GetWriterFormatCount();
		FbxExporter* lpExporter = FbxExporter::Create(pManager, "VIRFYN_Exporter");

		// 指定的文件格式非法，则默认置为 ascii
		if (fileFormat < 0 || fileFormat >= lFormatCount)
		{
			fileFormat = pManager->GetIOPluginRegistry()->GetNativeWriterFormat();
			for (int lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
			{
				if (pManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
				{
					std::string lFormatDescription = pManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
					if (lFormatDescription.find("ascii") != std::string::npos)
					{
						fileFormat = lFormatIndex;
						break;
					}
				}
			}
		}

		//char* lFilePath = nullptr;
		//FbxAnsiToUTF8(filePath.c_str(), lFilePath);	// FBXSDK 提供的字符编码转换 API，不转换的话导出的文件名是乱码
		//// 初始化导出器
		//if (!lpExporter->Initialize(lFilePath, fileFormat, pManager->GetIOSettings()))
		//	return false;
		//delete[] lFilePath;

		// 初始化导出器
		if (!lpExporter->Initialize(filePath.c_str(), fileFormat, pManager->GetIOSettings()))
			return false;

		lpExporter->Export(pScene);
		lpExporter->Destroy();

		return true;
	}

	bool  SetTimelineDefaultTimeSpan(FbxScene* pScene, const unsigned int& frame_start, const unsigned int& frame_end, const double& frequency)
	{
		if (!pScene)
			return false;

		FbxTime lTime_start,
			lTime_end;
		FbxTime::EMode lTimeMode = GetTimeMode(frequency);
		FbxTimeSpan lTimeSpan;

		pScene->GetGlobalSettings().SetTimeMode(lTimeMode);
		pScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeSpan);
		lTime_start.SetGlobalTimeMode(lTimeMode);
		lTime_start.SetFrame(frame_start, lTimeMode);
		lTime_end.SetGlobalTimeMode(lTimeMode);
		lTime_end.SetFrame(frame_end, lTimeMode);
		lTimeSpan.Set(lTime_start, lTime_end);
		pScene->GetGlobalSettings().SetTimelineDefaultTimeSpan(lTimeSpan);

		return true;
	}

	inline void  SetTranslation(FbxVector4& translation_fbx, const double translation_vd[3])
	{
		for (int i = 0; i < 3; i++)
			translation_fbx[i] = translation_vd[i];
		translation_fbx[3] = 0.0f;
	}

	inline void  SetQuaternion(FbxQuaternion& quaternion_fbx, const double quaternion_vd[3])
	{
		//【!】VIRDYN 格式的四元数的 w 在第 0 位，FBX 格式的四元数的 w 在第 3 位
		quaternion_fbx[0] = quaternion_vd[1];
		quaternion_fbx[1] = quaternion_vd[2];
		quaternion_fbx[2] = quaternion_vd[3];
		quaternion_fbx[3] = quaternion_vd[0];
	}

	void  TransformFilePath(std::string& filePath)
	{
		int lLength = MultiByteToWideChar(CP_OEMCP, 0, filePath.c_str(), -1, NULL, 0);
		wchar_t* lBuffer_wchar = new wchar_t[lLength + 1];
		wmemset(lBuffer_wchar, 0, lLength + 1);
		MultiByteToWideChar(CP_OEMCP, 0, filePath.c_str(), int(filePath.length()), lBuffer_wchar, lLength);
		char* lBuffer_char = new char[(lLength + 1) * 2];
		memset(lBuffer_char, 0, (lLength + 1) * 2);
		FbxWCToUTF8(lBuffer_wchar, lBuffer_char);
		filePath = lBuffer_char;
	}
}