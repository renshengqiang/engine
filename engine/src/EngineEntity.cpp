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
}

Entity::~Entity()
{
}

void Entity::setMeshPtr(MeshPtr mesh)
{
	m_mesh = mesh;
}	

}
