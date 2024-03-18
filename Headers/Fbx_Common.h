#include <fbxsdk.h> 
#include <string>


/* FBX 基础类
*	功能包含初始化内存管理器和场景、销毁内存管理器、设置部分导出设置、设置场景的坐标系、保存场景等
*/
namespace Fbx_Common
{
	/* 初始化内存管理器和场景
	*	pManager	[IN]:	内存管理器
	*	pScene		[IN]:	场景
	*	return		[OUT]:	初始化成功与否
	*/
	bool InitializeManagerAndScene(FbxManager*& pManager, FbxScene*& pScene);

	/* 初始化内存管理器和场景
	*	pManager	[IN]:	内存管理器
	*	return		[OUT]:	初始化成功与否
	*/
	bool InitializeManager(FbxManager*& pManager);

	/* 初始化场景
	*	pScene	[IN]:	场景
	*	return	[OUT]:	初始化成功与否
	*/
	bool InitializeScene(FbxManager*& pManager, FbxScene*& pScene);

	/* 销毁内存管理器
	*	pManager	[IN]:	内存管理器
	*/
	void DestroyManager(FbxManager*& pManager);

	/* 销毁场景
	*	pScene	[IN]:	场景
	*/
	void DestroyScene(FbxScene*& pScene);

	/* 设置部分导入设置
	*	material		[IN]:	是否导出材质
	*	texture			[IN]:	是否导出纹理
	*	link			[IN]:	是否导出链接
	*	shape			[IN]:	是否导出外形（形变器）
	*	gobo			[IN]:	是否导出？
	*	animation		[IN]:	是否导出动画
	*	globalSetting	[IN]:	是否导出设置
	*/
	void SetImportBoolProp(FbxManager* const pManager, const bool& material, const bool& texture, const bool& link,
		const bool& shape, const bool& gobo, const bool& animation, const bool& globalSetting);

	/* 设置部分导出设置
	*	material		[IN]:	是否导出材质
	*	texture			[IN]:	是否导出纹理
	*	embedded		[IN]:	是否导出嵌入的媒体素材
	*	shape			[IN]:	是否导出外形（形变器）
	*	gobo			[IN]:	是否导出？
	*	animation		[IN]:	是否导出动画
	*	globalSetting	[IN]:	是否导出设置
	*/
	void SetExportBoolProp(FbxManager* const pManager, const bool& material, const bool& texture, const bool& embedded,
		const bool& shape, const bool& gobo, const bool& animation, const bool& globalSetting);

	/* 加载 FBX 文件到场景中
	*	pManager	[IN]:	内存管理器
	*	pScene		[IN]:	场景
	*	filePath	[IN]:	FBX 文件路径
	*	return		[OUT]:	加载成功与否
	*/
	bool LoadFbxFile(FbxManager* pManager, FbxScene* pScene, const std::string& filePath);

	/* 设置场景的坐标系相关信息
	*	pScene			[IN]:	指定的场景
	*	vector_up		[IN]:	上轴，详细用法请查看 FBXSDK
	*	vector_front	[IN]:	前轴，详细用法请查看 FBXSDK
	*	coordSystem		[IN]:	手系，分右手系和左手系
	*/
	bool SetCoordSystem(FbxScene* const pScene, const FbxAxisSystem::EUpVector& vector_up = FbxAxisSystem::EUpVector::eZAxis,
		const FbxAxisSystem::EFrontVector& vector_front = FbxAxisSystem::EFrontVector::eParityOdd, const FbxAxisSystem::ECoordSystem& coordSystem = FbxAxisSystem::ECoordSystem::eRightHanded);

	/* 获取场景的坐标系相关信息
	*	pScene			[IN]:	指定的场景
	*	vector_up		[IN]:	上轴，详细用法请查看 FBXSDK
	*	sign_up			[IN]:	上轴的正负，详细用法请查看 FBXSDK
	*	vector_front	[IN]:	前轴，详细用法请查看 FBXSDK
	*	sign_front		[IN]:	前轴的正负，详细用法请查看 FBXSDK
	*	coordSystem		[IN]:	手系，分右手系和左手系
	*/
	bool GetCoordSystem(FbxScene* const pScene, FbxAxisSystem::EUpVector& vector_up, int& sign_up,
		FbxAxisSystem::EFrontVector& vector_front, int& sign_front, FbxAxisSystem::ECoordSystem& coordSystem);

	/* 设置场景的长度单位
	*	pScene	[IN]:	指定的场景
	*	unit	[IN]:	长度单位
	*/
	bool SetUnit(FbxScene* const pScene, const FbxSystemUnit& unit);

	/* 保存场景到 FBX 文件中
	*	pManager	[IN]:	指定的内存管理器
	*	pScene		[IN]:	指定的场景
	*	filePath	[IN]:	保存路径
	*	fileFormat	[IN]:	FBX 文件格式
	*	return		[OUT]:	保存成功与否
	*/
	bool SaveScene(FbxManager* const pManager, FbxDocument* const pScene, const std::string& filePath, int fileFormat = -1);

	/* 比较两个数是否几乎相同，主要用于浮点值
	*	lhs		[IN]:	数1
	*	rhs		[IN]:	数2
	*	return	[OUT]:	是否几乎相同
	*/
	template <typename T, typename U>
	bool IsAlmostEqual(const T& lhs, const U& rhs)
	{
		const double lEpsilon = 1e-6;	// 阈值
		return std::abs(lhs - rhs) < lEpsilon;
	}

	/* 将帧率转换为时间模式
	*	frequency	[IN]:	帧率
	*	return		[OUT]:	时间模式
	*/
	template <typename T>
	FbxTime::EMode GetTimeMode(const T& frequency)
	{
		typedef FbxTime::EMode EMode;
		if (IsAlmostEqual(frequency, 30.0))
			return EMode::eFrames30;
		else if (IsAlmostEqual(frequency, 60.0))
			return EMode::eFrames60;
		else if (IsAlmostEqual(frequency, 72.0))
			return EMode::eFrames72;
		else if (IsAlmostEqual(frequency, 96.0))
			return EMode::eFrames96;
		else if (IsAlmostEqual(frequency, 100.0))
			return EMode::eFrames100;
		else if (IsAlmostEqual(frequency, 120.0))
			return EMode::eFrames120;
		else
			return EMode::eFrames60;
	}

	/* 设置默认时间线
	*	frame_start	[IN]:	起始帧
	*	frame_end	[IN]:	结束帧
	*	frequency	[IN]:	帧率
	*/
	bool SetTimelineDefaultTimeSpan(FbxScene* pScene, const unsigned int& frame_start, const unsigned int& frame_end, const double& frequency);

	/* 将 VIRDYN 格式的坐标转换为 FBX 格式的坐标
	*	【!】FbxVector4 作为坐标使用时，第 3 位为 0.0f
	*	translation_fbx	[IN/OUT]:	FBX 格式的坐标
	*	translation_vd	[IN]:		VIRDYN 格式的坐标
	*/
	inline void SetTranslation(FbxVector4& translation_fbx, const double translation_vd[3]);

	/* 将 VIRDYN 格式的四元数转换为 FBX 格式的四元数
	*	【!】VIRDYN 格式的四元数的 w 在第 0 位，FBX 格式的四元数的 w 在第 3 位
	*	quaternion_fbx	[IN/OUT]:	FBX 格式的四元数
	*	quaternion_vd	[IN]:		VIRDYN 格式的四元数
	*/
	inline void SetQuaternion(FbxQuaternion& quaternion_fbx, const double quaternion_vd[3]);

	/* 将 char 存储的 char 类型文件路径字符串转为 char 存储的 wchar 类型（可供 FBXSDK 识别的）的文件路径字符串
	* filePath	[IN/OUT]:	文件路径
	*/
	void TransformFilePath(std::string& filePath);
};