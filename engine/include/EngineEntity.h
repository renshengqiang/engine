#ifndef _ENGINE_ENTITY_H
#define _ENGINE_ENTITY_H
#include <EngineMoveableObject.h>
#include <EngineSubEntity.h>
#include <EngineMesh.h>
#include <EngineAxisAlignedBox.h>
#include <EngineRenderQueue.h>
#include <string>

namespace Engine
{
class Matrix4f;
class Entity: public MoveableObject
{
typedef std::vector<RenderablePtr> SubEntityVec;
typedef SubEntityVec::iterator		SubEntityIter;

public:
	Entity();
	Entity(const std::string& meshFile);
	~Entity();

	void addToRenderQueue(const Matrix4f &transMatrix, RenderQueue &renderQueue);
	virtual const AxisAlignedBox& getBoundingBox() const { return m_boundingBox;}
	void setMeshPtr(MeshPtr mesh);
	
private:
	MeshPtr		m_mesh;							//对应的Mesh
	SubEntityVec	m_subEntityVec;					//对应的SubEntity
};
}

#endif
