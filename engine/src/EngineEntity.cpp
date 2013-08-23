#include "EngineEntity.h"
#include "EngineAxisAlignedBox.h"
#include "EngineMeshManager.h"

namespace Engine
{
Entity::Entity()
{
	m_visible = true;
}

Entity::Entity(const std::string& meshFile)
{
	m_mesh = MeshManager::getSingleton().createMesh(meshFile);
	m_visible = true;

	int numSubMesh = m_mesh->numSubMesh();
	m_subEntityVec.resize(numSubMesh);
	for(int i=0; i<numSubMesh; ++i)
	{
		m_subEntityVec[i].reset(new SubEntity(this, m_mesh->getSubMesh(i)));
	}

	m_boundingBox = m_mesh->getBoundingBox();
}

Entity::~Entity()
{
}

void Entity::addToRenderQueue(const Matrix4f &transMatrix, RenderQueue &renderQueue)
{
	for(unsigned i=0; i<m_subEntityVec.size(); ++i)
	{
		renderQueue.addItem(transMatrix, m_subEntityVec[i]);
	}
}
void Entity::setMeshPtr(MeshPtr mesh)
{
	m_mesh = mesh;
}	

}
