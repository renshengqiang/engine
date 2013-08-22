#ifndef _ENGINE_ENTITY_H
#define _ENGINE_ENTITY_H
#include <EngineMoveableObject.h>
#include <EngineMesh.h>
#include <EngineAxisAlignedBox.h>
#include <EngineSubEntity.h>
#include <string>

namespace Engine
{
class Entity: public MoveableObject
{
typedef std::vector<SubEntityPtr> SubEntityVec;
typedef SubEntityVec::iterator		SubEntityIter;

public:
	Entity();
	Entity(const std::string& meshFile);
	~Entity();
	
	virtual const AxisAlignedBox& getBoundingBox() const { return m_boundingBox;}
	void setMeshPtr(MeshPtr mesh);
	
private:
	MeshPtr		m_mesh;							//对应的Mesh
	SubEntityVec	m_subEntityVec;					//对应的SubEntity
};
}

#endif
