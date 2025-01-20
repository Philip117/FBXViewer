#include <queue>
#include <string>
#include <algorithm>
#include "FBX_Extraction.h"


namespace Fbx_Extraction
{
	std::vector<FbxNode*> GetModelRoots(FbxScene* pScene)
	{
		// 用广度优先搜索获取可选模型名称
		bool lIsHips = false,						// 当前节点是否为 Hips 关节
			lHasAnotherHips = false,				// 该节点下是否还有其它 Hips 关节
			lHasMeshInSameLayer = false;			// 同层下有没有 Mesh 节点
		std::queue<FbxNode*> lNodeQueue,			// 用于找到 Hips 关节的队列
			lNodeQueue_temp;						// 用于找到 Hips 关节后，第二次找 Hips 关节的队列
		FbxNode* lpNode = nullptr,					// 节点指针
			* lpNode_hips = nullptr,					// Hips 关节指针
			* lpNode_model = nullptr;				// 模型根节点
		std::string lName_node;						// 临时的单个节点名
		std::vector<FbxNode*> lModelRoots;

		lModelRoots.clear();
		lNodeQueue.push(pScene->GetRootNode());
		// 该循环用于寻找模型
		while (!lNodeQueue.empty())
		{
			lpNode = lNodeQueue.front();
			lpNode_hips = lpNode_model = nullptr;
			lNodeQueue.pop();
			//std::cout << lpNode->GetName() << std::endl;

			// 判断当前节点是否非 Mesh 节点，是的话再判断是否为 Hips 关节
			if (lpNode->GetNodeAttribute() && lpNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				lName_node = lpNode->GetName();
				std::transform(lName_node.begin(), lName_node.end(), lName_node.begin(), ::tolower);

				// 该节点是 Hips 关节	
				if (lName_node.find("hip") != std::string::npos)	// 用“hip”不用“hips”是因为 CC4 标准没有“hips”只有“hip”
				{
					lpNode_hips = lpNode;
					lHasMeshInSameLayer = false;

					// 向上溯源，直到找到一层有 Mesh 节点的层，将该层的父节点暂定为模型根节点
					// 该循环用于寻找一个模型的根节点
					while (!lHasMeshInSameLayer && lpNode->GetParent())
					{
						lpNode = lpNode->GetParent();
						for (int i = 0; i < lpNode->GetChildCount(); i++)
						{
							if (lpNode->GetChild(i)->GetNodeAttribute() && lpNode->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
							{
								lHasMeshInSameLayer = true;
								lpNode_model = lpNode;
								break;
							}
						}
					}

					// 如果找到了同层有 Mesh 节点的，则很可能是个带蒙皮的动画，就从该层的父节点（lpNode_model）开始广度优先遍历
					if (lHasMeshInSameLayer)
					{
						lHasAnotherHips = false;
						std::queue<FbxNode*>().swap(lNodeQueue_temp);
						for (int i = 0; i < lpNode->GetChildCount(); i++)
							lNodeQueue_temp.push(lpNode_model->GetChild(i));
						while (!lNodeQueue_temp.empty())
						{
							lpNode = lNodeQueue_temp.front();
							lNodeQueue_temp.pop();

							// 不对 Mesh 节点进行广度优先遍历
							if (!lpNode->GetNodeAttribute() || (lpNode->GetNodeAttribute() && lpNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh))
							{
								lName_node = lpNode->GetName();
								std::transform(lName_node.begin(), lName_node.end(), lName_node.begin(), ::tolower);

								// 找到一个 Hips 关节
								if (lName_node.find("hip") != std::string::npos)
								{
									// 如果该 Hips 关节和向上溯源时的 Hips 关节是同一个，则跳过，不继续对它进行广度优先遍历
									if (lpNode == lpNode_hips)
									{
										continue;
									}
									// 如果该 Hips 关节不同于向上溯源时的 Hips，说明本次广度优先遍历的起点不适合作为模型的根节点（模型制作者可能把多个模型的骨架和蒙皮都放在了同层）
									else
									{
										lHasAnotherHips = true;
										break;
									}
								}
								// 如果不是 Hips 关节的话，将其子节点入队
								else
								{
									for (int i = 0; i < lpNode->GetChildCount(); i++)
										lNodeQueue_temp.push(lpNode->GetChild(i));
								}
							}
						}

						// 如果发现该层的父节点（lpNode）下可能有多个 Hips 关节，则说明该层的父节点（lpNode）不适合作为模型的根节点（模型制作者可能把多个模型的骨架和蒙皮都放在了同层）
						if (lHasAnotherHips)
						{
							lModelRoots.clear();
							return lModelRoots;
						}

						// 既是 Hips 关节，向上溯源能找到同层有 Mesh 节点的，而且该层下还没有其它 Hips 关节，则认定为 1 个模型
						lModelRoots.push_back(lpNode_model);
					}
					// 如果向上溯源没找到 Mesh 节点，则说明很可能是个骨骼动画，或者 Mesh 节点的根节点与 Hips 或其父节点不同同层但和其它关节同层
					else
					{
						//// 如果 Hips 关节还有父节点，则将其父节点作为模型根节点
						//if (lpNode_hips->GetParent() && lpNode_hips->GetParent()->GetNodeAttribute()
						//	&& lpNode_hips->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
						//	lpNode_model = lpNode_hips->GetParent();
						//// 若 Hips 关节自身就是 FBX 根节点，则直接作为模型根节点（但 FBX 一般还会有 RootNode 做根节点）
						//else
						//	lpNode_model = lpNode_hips;
						lpNode_model = lpNode_hips;
						lModelRoots.push_back(lpNode_model);
						continue;
					}
				}
			}

			if (!lpNode_model)
			{
				// 判断当前节点是否为 Mesh 节点，不是的话就把子节点入队
				if (!lpNode->GetNodeAttribute() || (lpNode->GetNodeAttribute() && lpNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh))
				{
					for (int i = 0; i < lpNode->GetChildCount(); i++)
						lNodeQueue.push(lpNode->GetChild(i));
				}
			}
		}

		return lModelRoots;
	}
}