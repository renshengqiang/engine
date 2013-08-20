#ifndef _ENGINE_MESH_H
#define _ENGINE_MESH_H
#include <EngineVector.h>
#include <EngineAxisAlignedBox.h>
#include <string>
#include <vector>

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
	
	std::string getName(void) { return m_name;}
	const AxisAlignedBox& getBoundingBox(void) const { return m_AABB;}
	void setBoundingBox(const Vector3f& min, const Vector3f& max)
	{
		m_AABB.setMinimum(min);
		m_AABB.setMaximum(max);
	}
	void render(SimpleMeshEffect& effect);
	
private:
	/// name of the mesh
	std::string m_name;
	/// Local bounding box volume
	AxisAlignedBox m_AABB;
	/// submesh
	std::vector<SubMesh*> m_subMeshes;
	/// texture names
	std::vector<std::string> m_texturNameVec;
	///
	bool m_finalized;	
};
}

#endif
