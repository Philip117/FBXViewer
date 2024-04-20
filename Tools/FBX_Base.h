#include <fbxsdk.h> 
#include <string>


/* FBX 基础类
*	功能包含初始化内存管理器和场景、销毁内存管理器、设置部分导出设置、设置场景的坐标系、保存场景等
*/
namespace Fbx_Base
{
	/* 获取 FBXSDK 版本
	* major		[IN/OUT]:	大版本号
	* minor		[IN/OUT]:	小版本号
	* revision	[IN/OUT]:	修订版本号
	*/
	void GetSdkVersion(int& major, int& minor, int& revision);

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

	/* 保存场景到 FBX 文件中
	*	pManager	[IN]:	指定的内存管理器
	*	pScene		[IN]:	指定的场景
	*	filePath	[IN]:	保存路径
	*	fileFormat	[IN]:	FBX 文件格式
	*	return		[OUT]:	保存成功与否
	*/
	bool SaveScene(FbxManager* const pManager, FbxDocument* const pScene, const std::string& filePath, int fileFormat = -1);

	/* 将 char 存储的 char 类型文件路径字符串转为 char 存储的 wchar 类型（可供 FBXSDK 识别的）的文件路径字符串
	* filePath	[IN/OUT]:	文件路径
	*/
	void TransformFilePath(std::string& filePath);
};