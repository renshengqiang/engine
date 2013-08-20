#ifndef _ENGINE_SUBMESH_H
#define _ENGINE_SUBMESH_H
#include <EngineVector.h>
#include <EngineRenderOp.h>
#include <vector>
#define MAX_BONE_ATTACHED 4

namespace Engine
{

class AttachedBone
{
public:
	float boneId[MAX_BONE_ATTACHED];
	float weight[MAX_BONE_ATTACHED];
	
	AttachedBone()
	{
		for(int i=0; i<MAX_BONE_ATTACHED;++i){
			boneId[i]=0;
			weight[i]=0;
		}
	}
};

class Texture;
class SimpleMeshEffect;
class SubMesh
{
public:
	SubMesh();
	~SubMesh();

	//��������
	unsigned getVertexNum();
	
	//��ȡһ�������Ӧ�Ĺ����ڵ������
	unsigned getVertexAttachedBoneNum(unsigned vertexIndex);

	//��ȡһ�������Ӧ�Ĺ����ڵ����Ϣ
	AttachedBone* getVertexAttachedBones(unsigned vertexIndex);
	

	//���Ӷ���
	void addCoord(const Vector3f &coord);
	
	//���������ļ�
	void setTextureName(const char *filename);
	
	//������������
	void addTextureCoord(const Vector2f &textureCoord);
	
	//��������
	void addIndex(int index1, int index2, int index3);
	
	//���ӹ�����(index �Ƕ�������boneId �ǹ����ڵ����� weight�����Զ���Ӱ��Ȩ��)
	void addBoneData(int index, unsigned short boneId, float weight);
	
	//����ΪFBO��ʹ��Ӳ��������Ⱦ
	void finalize();

	//��Ⱦ����
	void render(SimpleMeshEffect& effect);
	
private:
	//CPU�д�ŵ����ݣ����ڳ�ʼ��
	std::vector<Vector3f> m_coordVec;				//��������
	std::vector<Vector2f> m_textureCoordVec;		//������������	
	std::vector<int> m_indexVec;					//��������
	std::vector<AttachedBone> m_attachedBoneVec;	//�󶨹�������
	std::vector<unsigned> m_boneNumVec;			//ÿ������󶨵Ĺ����ڵ������(�������������ڰ�����ʼ��attachedBoneVec)
	Texture *mp_texture;							//��Ӧ������ͼ��

	//GPU�д�ŵ����ݣ�����Ӳ������
	VERTEX_OBJ *mp_vertexObject;				//���㻺��������
	INDEX_OBJ *mp_indexObject;					//��������������
	PIXEL_OBJ *mp_pixelObject;					//������������
};
}

#endif
