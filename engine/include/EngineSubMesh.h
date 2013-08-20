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

	//顶点数量
	unsigned getVertexNum();
	
	//获取一个顶点对应的骨骼节点的数量
	unsigned getVertexAttachedBoneNum(unsigned vertexIndex);

	//获取一个顶点对应的骨骼节点的信息
	AttachedBone* getVertexAttachedBones(unsigned vertexIndex);
	

	//增加顶点
	void addCoord(const Vector3f &coord);
	
	//设置纹理文件
	void setTextureName(const char *filename);
	
	//增加纹理坐标
	void addTextureCoord(const Vector2f &textureCoord);
	
	//增加索引
	void addIndex(int index1, int index2, int index3);
	
	//增加骨骼绑定(index 是顶点索引boneId 是骨骼节点索引 weight骨骼对顶点影响权重)
	void addBoneData(int index, unsigned short boneId, float weight);
	
	//编译为FBO，使用硬件加速渲染
	void finalize();

	//渲染操作
	void render(SimpleMeshEffect& effect);
	
private:
	//CPU中存放的数据，用于初始化
	std::vector<Vector3f> m_coordVec;				//顶点数据
	std::vector<Vector2f> m_textureCoordVec;		//纹理坐标数据	
	std::vector<int> m_indexVec;					//索引数据
	std::vector<AttachedBone> m_attachedBoneVec;	//绑定骨骼数据
	std::vector<unsigned> m_boneNumVec;			//每个顶点绑定的骨骼节点的数量(辅助变量，用于帮助初始化attachedBoneVec)
	Texture *mp_texture;							//对应的纹理图像

	//GPU中存放的数据，用于硬件加速
	VERTEX_OBJ *mp_vertexObject;				//顶点缓冲区对象
	INDEX_OBJ *mp_indexObject;					//索引缓冲区对象
	PIXEL_OBJ *mp_pixelObject;					//纹理缓冲区对象
};
}

#endif
