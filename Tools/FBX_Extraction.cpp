#include <queue>
#include <string>
#include <algorithm>
#include "FBX_Extraction.h"


namespace Fbx_Extraction
{
	std::vector<FbxNode*> GetModelRoots(FbxScene* pScene)
	{
		// �ù������������ȡ��ѡģ������
		bool lIsHips = false,						// ��ǰ�ڵ��Ƿ�Ϊ Hips �ؽ�
			lHasAnotherHips = false,				// �ýڵ����Ƿ������� Hips �ؽ�
			lHasMeshInSameLayer = false;			// ͬ������û�� Mesh �ڵ�
		std::queue<FbxNode*> lNodeQueue,			// �����ҵ� Hips �ؽڵĶ���
			lNodeQueue_temp;						// �����ҵ� Hips �ؽں󣬵ڶ����� Hips �ؽڵĶ���
		FbxNode* lpNode = nullptr,					// �ڵ�ָ��
			* lpNode_hips = nullptr,					// Hips �ؽ�ָ��
			* lpNode_model = nullptr;				// ģ�͸��ڵ�
		std::string lName_node;						// ��ʱ�ĵ����ڵ���
		std::vector<FbxNode*> lModelRoots;

		lModelRoots.clear();
		lNodeQueue.push(pScene->GetRootNode());
		// ��ѭ������Ѱ��ģ��
		while (!lNodeQueue.empty())
		{
			lpNode = lNodeQueue.front();
			lpNode_hips = lpNode_model = nullptr;
			lNodeQueue.pop();
			//std::cout << lpNode->GetName() << std::endl;

			// �жϵ�ǰ�ڵ��Ƿ�� Mesh �ڵ㣬�ǵĻ����ж��Ƿ�Ϊ Hips �ؽ�
			if (lpNode->GetNodeAttribute() && lpNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				lName_node = lpNode->GetName();
				std::transform(lName_node.begin(), lName_node.end(), lName_node.begin(), ::tolower);

				// �ýڵ��� Hips �ؽ�	
				if (lName_node.find("hip") != std::string::npos)	// �á�hip�����á�hips������Ϊ CC4 ��׼û�С�hips��ֻ�С�hip��
				{
					lpNode_hips = lpNode;
					lHasMeshInSameLayer = false;

					// ������Դ��ֱ���ҵ�һ���� Mesh �ڵ�Ĳ㣬���ò�ĸ��ڵ��ݶ�Ϊģ�͸��ڵ�
					// ��ѭ������Ѱ��һ��ģ�͵ĸ��ڵ�
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

					// ����ҵ���ͬ���� Mesh �ڵ�ģ���ܿ����Ǹ�����Ƥ�Ķ������ʹӸò�ĸ��ڵ㣨lpNode_model����ʼ������ȱ���
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

							// ���� Mesh �ڵ���й�����ȱ���
							if (!lpNode->GetNodeAttribute() || (lpNode->GetNodeAttribute() && lpNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh))
							{
								lName_node = lpNode->GetName();
								std::transform(lName_node.begin(), lName_node.end(), lName_node.begin(), ::tolower);

								// �ҵ�һ�� Hips �ؽ�
								if (lName_node.find("hip") != std::string::npos)
								{
									// ����� Hips �ؽں�������Դʱ�� Hips �ؽ���ͬһ�������������������������й�����ȱ���
									if (lpNode == lpNode_hips)
									{
										continue;
									}
									// ����� Hips �ؽڲ�ͬ��������Դʱ�� Hips��˵�����ι�����ȱ�������㲻�ʺ���Ϊģ�͵ĸ��ڵ㣨ģ�������߿��ܰѶ��ģ�͵ĹǼܺ���Ƥ��������ͬ�㣩
									else
									{
										lHasAnotherHips = true;
										break;
									}
								}
								// ������� Hips �ؽڵĻ��������ӽڵ����
								else
								{
									for (int i = 0; i < lpNode->GetChildCount(); i++)
										lNodeQueue_temp.push(lpNode->GetChild(i));
								}
							}
						}

						// ������ָò�ĸ��ڵ㣨lpNode���¿����ж�� Hips �ؽڣ���˵���ò�ĸ��ڵ㣨lpNode�����ʺ���Ϊģ�͵ĸ��ڵ㣨ģ�������߿��ܰѶ��ģ�͵ĹǼܺ���Ƥ��������ͬ�㣩
						if (lHasAnotherHips)
						{
							lModelRoots.clear();
							return lModelRoots;
						}

						// ���� Hips �ؽڣ�������Դ���ҵ�ͬ���� Mesh �ڵ�ģ����Ҹò��»�û������ Hips �ؽڣ����϶�Ϊ 1 ��ģ��
						lModelRoots.push_back(lpNode_model);
					}
					// ���������Դû�ҵ� Mesh �ڵ㣬��˵���ܿ����Ǹ��������������� Mesh �ڵ�ĸ��ڵ��� Hips ���丸�ڵ㲻ͬͬ�㵫�������ؽ�ͬ��
					else
					{
						//// ��� Hips �ؽڻ��и��ڵ㣬���丸�ڵ���Ϊģ�͸��ڵ�
						//if (lpNode_hips->GetParent() && lpNode_hips->GetParent()->GetNodeAttribute()
						//	&& lpNode_hips->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
						//	lpNode_model = lpNode_hips->GetParent();
						//// �� Hips �ؽ�������� FBX ���ڵ㣬��ֱ����Ϊģ�͸��ڵ㣨�� FBX һ�㻹���� RootNode �����ڵ㣩
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
				// �жϵ�ǰ�ڵ��Ƿ�Ϊ Mesh �ڵ㣬���ǵĻ��Ͱ��ӽڵ����
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