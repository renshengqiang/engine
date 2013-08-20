#include "EngineSubMesh.h"
#include "EngineTexture.h"
#include "EngineSimpleMeshEffect.h"
#include <stdio.h>

namespace Engine
{
	SubMesh::SubMesh():
		mp_texture(NULL),
		mp_vertexObject(NULL),
		mp_indexObject(NULL),
		mp_pixelObject(NULL)
	{
	}
		
	SubMesh::~SubMesh()
	{
		if(mp_vertexObject)
		{
			DestroyVertexObject(mp_vertexObject);
		}
		if(mp_indexObject)
		{
			DestroyIndexObject(mp_indexObject);
		}
		if(mp_texture)
		{
			delete mp_texture;
		}
		//FIXME: destroy the pixel object
		//��ʵPixelObjectû�д���ڵļ�ֵ������ɾ��
	}

	unsigned SubMesh::getVertexNum()
	{
		return m_coordVec.size();
	}
	
	//��ȡһ�������Ӧ�Ĺ����ڵ������
	unsigned SubMesh::getVertexAttachedBoneNum(unsigned vertexIndex)
	{
		return m_boneNumVec[vertexIndex];
	}

	//��ȡһ�������Ӧ�Ĺ����ڵ����Ϣ
	AttachedBone* SubMesh::getVertexAttachedBones(unsigned vertexIndex)
	{
		return &(m_attachedBoneVec[vertexIndex]);
	}
	

	//���Ӷ���
	void SubMesh::addCoord(const Vector3f &coord)
	{
		m_coordVec.push_back(coord);
	}
	
	//������������
	void SubMesh::addTextureCoord(const Vector2f &textureCoord)
	{
		m_textureCoordVec.push_back(textureCoord);
	}
	
	//��������
	void SubMesh::addIndex(int index1, int index2, int index3)
	{
		m_indexVec.push_back(index1);
		m_indexVec.push_back(index2);
		m_indexVec.push_back(index3);
	}
	
	//���������ļ�
	void SubMesh::setTextureName(const char *filename)
	{
		mp_texture = new Texture(filename);
	}
			
	//���ӹ�����(index �Ƕ�������boneId �ǹ����ڵ����� weight�����Զ���Ӱ��Ȩ��)
	void SubMesh::addBoneData(int index, unsigned short boneId, float weight)
	{
		int n = m_boneNumVec[index];
		m_attachedBoneVec[index].boneId[n] = boneId;
		m_attachedBoneVec[index].weight[n] = weight;
	}
	
	//����ΪFBO��ʹ��Ӳ��������Ⱦ
	void SubMesh::finalize()
	{
		mp_vertexObject = CreateVertexObject(COORD_3|TEXTURE_2, m_coordVec.size());
		VertexObjectPushElementAll(mp_vertexObject, COORD_3, (float*)&m_coordVec[0]);
		VertexObjectPushElementAll(mp_vertexObject, TEXTURE_2, (float*)&m_textureCoordVec[0]);
		//VertexObjectPushElementAll(mp_vertexObject, BONE_4, (float *)&m_attachedBoneVec[0]);
		VertexObjectEnd(mp_vertexObject);

		mp_indexObject = CreateIndexObject(m_indexVec.size(),(unsigned int *)&m_indexVec[0]);

		bool ret = mp_texture->load();
		assert(ret == true);
		mp_pixelObject = CreatePixelObject2(mp_texture);
	}

	//��Ⱦ����
	void SubMesh::render(SimpleMeshEffect& effect)
	{
		DrawSimpleMesh(mp_indexObject, mp_vertexObject, mp_pixelObject, effect);
	}
}
