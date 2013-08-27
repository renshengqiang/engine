#include "EngineOctreeRaySceneQuery.h"
#include "EngineOctreeSceneManager.h"
#include "EngineMoveableObject.h"
#include "EngineSceneNode.h"
#include <algorithm>

namespace Engine
{
RaySceneQueryResult& OctreeRaySceneQuery::execute()
{
	// Clear without freeing the vector buffer
	m_result.clear();

	std::vector<SceneNode*> list;

	static_cast<OctreeSceneManager *>(mp_sceneManager)->findNodesIn(m_ray, list, 0);
   
	for(std::vector<SceneNode*>::iterator iter = list.begin(); iter != list.end(); ++ iter)
	{
		std::pair<bool, float> result = m_ray.intersects((*iter)->getWorldBoundingBox());
		MoveableObject *pMoveable = (*iter)->getAttachedMoveableObject();
		if(result.first && pMoveable && pMoveable->visible())
		{
			queryResult(pMoveable, result.second);
		}
	}

	if (m_sortByDistance)
	{
		if (m_maxResults != 0 && m_maxResults < m_result.size())
		{
			// Partially sort the N smallest elements, discard others
			std::partial_sort(m_result.begin(), m_result.begin()+m_maxResults, m_result.end());
			m_result.resize(m_maxResults);
		}
		else
		{
			// Sort entire result array
			std::sort(m_result.begin(), m_result.end());
		}
	}

	return m_result;
}
}
