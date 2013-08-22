#ifndef _ENGINE_MESH_H
#define _ENGINE_MESH_H
#include <EngineVector.h>
#include <EngineAxisAlignedBox.h>
#include <string>
#include <vector>
#include <tr1/memory>

namespace Engine
{
class SubMesh;
class SimpleMeshEffect;
class  Mesh
{
public:
	Mesh(const std::string& filename);
	~Mesh();

	//从文件中load数据初始化Mesh
	bool load();

	void addSubMesh(SubMesh *pSubMesh);
	int numSubMesh(){ return m_subMeshes.size();}
	SubMesh	*getSubMesh(int index);
	
	std::string getName(void) { return m_name;}
	
	const AxisAlignedBox& getBoundingBox(void) const { return m_AABB;}
	void setBoundingBox(const Vector3f& min, const Vector3f& max)
	{
		m_AABB.setMinimum(min);
		m_AABB.setMaximum(max);
	}

	/// used by rending thread
	bool _finalize();
	
private:
	std::string m_name;							//mesh名
	AxisAlignedBox m_AABB;						//局部坐标的包围盒
	std::vector<SubMesh*> m_subMeshes;			//submeshes
	std::vector<std::string> m_texturNameVec;		//texture names
};

typedef std::tr1::shared_ptr<Mesh>  MeshPtr;
typedef std::tr1::weak_ptr<Mesh> MeshWeakPtr;
}



#endif
